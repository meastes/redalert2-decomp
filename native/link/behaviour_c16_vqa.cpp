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

void __fastcall VQA_Reset(VQAHandle *vqa)
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

VQAConfig _defaultconfig;

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
    VQAConfig &c = _defaultconfig;
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

// ---------------------------------------------------------------------------------------------------
// The half-height (interlaced) hi-colour block decoders VQAClass::Play_VQA installs with VQA option 11
// (lane C16, the owner's item (1); nothing in the game sets the option, but a mod's setup may): each
// 4x4 block row of the buffer receives two lines, its rows 0 and 2, from rows 0 and 2 of the 32-byte
// codeword (except where noted). Written from the original's instructions (0x0070BBC0, 0x0070C0A0);
// 0x0070C0A0 is the parked draft of src/recovered/unvqtblc.cpp (RA2_NON_MATCHING, score 75.55),
// checked against them. width is in pixels; the command stream is 16-bit words.
// ---------------------------------------------------------------------------------------------------
#include "shared/unvq_table.hpp"
#include "shared/movie.hpp"

namespace {
inline unsigned int pair_at(unsigned char const *cb)
{
    return (unsigned int)*(unsigned short const *)cb | (unsigned int)*(unsigned short const *)(cb + 2) << 16;
}
}

// UnVQ2_4x2_Table 0x0070BBC0: commands as UnVQ2_4x4_Table 0x0070B820 (the top four bits: 0 a colour
// from HicolorTable for the count's blocks, 0x1000 skip, 0x2000 one codeword repeated, 0x3000 the same
// with 0x8000 transparent, 0x5000/0x6000/0x7000 a colour, a codeword, a transparent codeword per
// block), two lines each.
void VQA_Unknown_0070BBC0(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer, RA2_ULONG blocks_per_row,
                          RA2_ULONG rows, RA2_ULONG width)
{
    uint32_t pitch = width * 2;                                   // bytes per pixel row
    unsigned char *end = buffer + rows * pitch * 4;
    unsigned char *row_start = buffer;
    unsigned char *row_end = buffer + blocks_per_row * 8;
    unsigned short *commands = (unsigned short *)pointers;
    unsigned char *out = buffer;
    while (out < end) {
        uint32_t token = *commands++;
        uint32_t count = token & 0x0fff;
        uint32_t opcode = token & 0xf000;
        if (opcode == 0x2000) {
            unsigned char const *cb = codebook + (uint32_t)*commands++ * 32;
            for (int line = 0; line < 2; ++line) {
                uint32_t first = pair_at(cb);
                cb += 4;
                for (uint32_t k = 0; k < count; ++k) *(uint32_t *)(out + 8 * k) = first;
                uint32_t second = pair_at(cb);
                cb += 12;
                for (uint32_t k = 0; k < count; ++k) *(uint32_t *)(out + 4 + 8 * k) = second;
                out += pitch * 2;
            }
            out += count * 8 - pitch * 4;
        } else if (opcode == 0x1000) {
            out += count * 8;
        } else if (opcode == 0) {
            unsigned short color = HicolorTable[*commands++];
            uint32_t pair = (uint32_t)color | (uint32_t)color << 16;
            for (uint32_t k = 0; k < count * 2; ++k) ((uint32_t *)out)[k] = pair;
            for (uint32_t k = 0; k < count * 2; ++k) ((uint32_t *)(out + pitch * 2))[k] = pair;
            out += count * 8;
        } else if (opcode == 0x3000) {
            unsigned short const *cb = (unsigned short const *)(codebook + (uint32_t)*commands++ * 32);
            for (int line = 0; line < 2; ++line) {
                unsigned short *pixel = (unsigned short *)out;
                for (int column = 0; column < 4; ++column, ++pixel, ++cb) {
                    unsigned short c = *cb;
                    if (!(c & 0x8000))
                        for (uint32_t k = 0; k < count; ++k) pixel[4 * k] = c;
                }
                cb += 4;                                              // the codeword's next row
                out += pitch * 2;
            }
            out += count * 8 - pitch * 4;
        } else if (opcode == 0x5000 || opcode == 0x6000 || opcode == 0x7000) {
            uint32_t line_step = (pitch >> 1) << 2;                   // two pixel rows
            for (; count > 0; --count, out += 8) {
                if (opcode == 0x5000) {
                    unsigned short color = HicolorTable[*commands++];
                    uint32_t pair = (uint32_t)color | (uint32_t)color << 16;
                    for (int line = 0; line < 2; ++line) {
                        ((uint32_t *)(out + line * line_step))[0] = pair;
                        ((uint32_t *)(out + line * line_step))[1] = pair;
                    }
                } else if (opcode == 0x6000) {
                    unsigned char const *cb = codebook + (uint32_t)*commands++ * 32;
                    for (int line = 0; line < 2; ++line) {
                        ((uint32_t *)(out + line * line_step))[0] = pair_at(cb);
                        ((uint32_t *)(out + line * line_step))[1] = pair_at(cb + 4);
                        cb += 16;
                    }
                } else {
                    // 0x7000: the codeword's rows 0 and 1 (no row skipped), transparent 0x8000.
                    unsigned short const *cb = (unsigned short const *)(codebook + (uint32_t)*commands++ * 32);
                    for (int line = 0; line < 2; ++line) {
                        unsigned short *pixel = (unsigned short *)(out + line * pitch * 2);
                        for (int column = 0; column < 4; ++column, ++cb)
                            if (!(*cb & 0x8000)) pixel[column] = *cb;
                    }
                }
            }
        }
        if (out == row_end) {
            row_start += pitch * 4;
            out = row_start;
            row_end = out + blocks_per_row * 8;
        }
    }
}

// UnVQ1_4x2_Table 0x0070C0A0: commands in the top three bits (0 skip the low byte's blocks; 0x2000
// one codeword (the low byte) repeated 2..64 times; 0xA000 the same with the count in the next byte
// and the 13-bit index; 0x4000 a run: the token's own low byte's codeword, its first two lines one row
// apart, then 2..64 codewords from the following bytes; 0x6000 four lines of a codeword and the next,
// two rows apart; 0x8000 the same transparent with 0x8000; 0xC000 a transparent codeword repeated
// the next byte's times).
void VQA_Unknown_0070C0A0(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer, RA2_ULONG blocks_per_row,
                          RA2_ULONG rows, RA2_ULONG width)
{
    uint32_t pitch = width * 2;
    unsigned char *end = buffer + rows * pitch * 4;
    unsigned char *out = buffer;
    unsigned char *row_start = buffer;
    unsigned char *row_end = row_start + blocks_per_row * 8;
    unsigned char *src = pointers;
    while (out < end) {
        uint32_t token = *(unsigned short *)src;
        src += 2;
        uint32_t opcode = token & 0xe000;
        uint32_t index = token & 0x1fff;
        if (opcode == 0) {
            out += 8 * (index & 0xff);
        } else if (opcode == 0x2000 || opcode == 0xa000) {
            uint32_t repeats, block;
            if (opcode == 0x2000) {
                repeats = ((index >> 7) & 0x3e) + 2;
                block = index & 0xff;
            } else {
                repeats = *src++;
                block = index;
            }
            unsigned char const *cb = codebook + block * 32;
            for (int line = 0; line < 2; ++line) {
                uint32_t first = pair_at(cb);
                uint32_t second = pair_at(cb + 4);
                cb += 16;
                for (uint32_t k = 0; k < repeats; ++k) *(uint32_t *)(out + 8 * k) = first;
                for (uint32_t k = 0; k < repeats; ++k) *(uint32_t *)(out + 4 + 8 * k) = second;
                out += pitch * 2;
            }
            out += 8 * repeats - pitch * 4;
        } else if (opcode == 0x4000) {
            uint32_t count = ((index >> 7) & 0x3e) + 2;
            unsigned char const *cb = codebook + (token & 0xff) * 32;
            unsigned char *pixel = out;
            for (int line = 0; line < 2; ++line) {
                *(uint32_t *)pixel = pair_at(cb);
                *(uint32_t *)(pixel + 4) = pair_at(cb + 4);
                cb += 16;
                pixel += 4 * (pitch >> 2);                            // one row, as the original
            }
            out += 8;
            for (uint32_t i = 0; i < count; ++i, out += 8) {
                unsigned char const *next = codebook + (uint32_t)*src++ * 32;
                unsigned char *p = out;
                for (int line = 0; line < 2; ++line) {
                    *(uint32_t *)p = pair_at(next);
                    *(uint32_t *)(p + 4) = pair_at(next + 4);
                    next += 16;
                    p += 8 * (pitch >> 2);                            // two rows
                }
            }
        } else if (opcode == 0x6000) {
            unsigned char const *cb = codebook + index * 32;
            for (int line = 0; line < 4; ++line) {
                *(uint32_t *)out = pair_at(cb);
                *(uint32_t *)(out + 4) = pair_at(cb + 4);
                cb += 16;
                out += pitch * 2;
            }
            out += 8 - pitch * 4;
        } else if (opcode == 0x8000) {
            unsigned short const *cb = (unsigned short const *)(codebook + index * 32);
            for (int line = 0; line < 4; ++line) {
                unsigned short *pixel = (unsigned short *)out;
                for (int column = 0; column < 4; ++column, ++cb)
                    if (!(*cb & 0x8000)) pixel[column] = *cb;
                cb += 4;
                out += pitch * 2;
            }
            out += 8 - pitch * 4;
        } else if (opcode == 0xc000) {
            uint32_t count = *src++;
            unsigned short const *cb = (unsigned short const *)(codebook + index * 32);
            for (int line = 0; line < 2; ++line) {
                unsigned short *pixel = (unsigned short *)out;
                for (int column = 0; column < 4; ++column, ++cb) {
                    unsigned short c = *cb;
                    if (!(c & 0x8000))
                        for (uint32_t k = 0; k < count; ++k) pixel[column + 4 * k] = c;
                }
                cb += 4;
                out += pitch * 2;
            }
            out += 8 * count - pitch * 4;
        }
        if (out == row_end) {
            row_start += pitch * 4;
            out = row_start;
            row_end = row_start + blocks_per_row * 8;
        }
    }
}

// ---------------------------------------------------------------------------------------------------
// The movie subtitle script's keyword table 0x007F62F0 and alignment names 0x007F6320 (lane C16):
// the original stores them (UTF-16 keywords and alignment names, handler addresses, alignment values);
// they are read from the user's game.exe when the first subtitle parser is made (native fixup in
// subtitle.cpp's SubTitleParser constructor), the keywords and names pointing into the mapped image,
// each handler address mapped to the native handler of that address. Before, both tables were zero
// placeholders: no keyword was recognised.
// ---------------------------------------------------------------------------------------------------
#include "shared/subtitle.hpp"
namespace ra2port { void *original_data_pointer(uint32_t va); }

struct SubTitleHook {
    unsigned short const *Token;
    void (__fastcall *Handler)(unsigned short *param, SubTitle *subtitle);
};
struct SubTitleAlignmentName {
    unsigned short const *Name;
    int Alignment;
};
SubTitleHook unknown_subtitle_hooks_7f62f0[8];
SubTitleAlignmentName unknown_subtitle_alignments_7f6320[8];

struct UnknownTimed_0069be40 {
    int unknown_00;
    unsigned Time;
};
unsigned __fastcall unknown_parse_timecode(unsigned short const *text);                           // 0x0069BD60
void __fastcall unknown_set_timecode(unsigned short const *text, UnknownTimed_0069be40 *target);  // 0x0069BE40 Duration
void __fastcall unknown_subtitle_parse_position(unsigned short *value, SubTitle *subtitle);       // 0x0069BE60
void __fastcall unknown_subtitle_parse_color(unsigned short *value, SubTitle *subtitle);          // 0x0069BEF0
void __fastcall unknown_subtitle_parse_text(unsigned short const *value, SubTitle *subtitle);     // 0x0069BF70

namespace {
// 0x0069BE30 "Time": the time code parsed is the subtitle's display time (stored even when 0).
void __fastcall subtitle_time(unsigned short *param, SubTitle *subtitle)
{
    subtitle->Set_Display_Time(unknown_parse_timecode(param));
}
void __fastcall subtitle_duration(unsigned short *param, SubTitle *subtitle)
{
    unknown_set_timecode(param, (UnknownTimed_0069be40 *)subtitle);
}
void __fastcall subtitle_text(unsigned short *param, SubTitle *subtitle)
{
    unknown_subtitle_parse_text(param, subtitle);
}
}

void Native_Subtitle_Tables()
{
    static bool filled = false;
    if (filled) return;
    filled = true;
    int hooks = 0;
    for (int i = 0; i < 7; ++i) {
        uint32_t entry[2];
        if (!ra2port::original_bytes(0x007F62F0u + 8u * (uint32_t)i, entry, sizeof entry) || entry[0] == 0) break;
        void (__fastcall *handler)(unsigned short *, SubTitle *) = 0;
        switch (entry[1]) {
        case 0x0069BE30u: handler = subtitle_time; break;
        case 0x0069BE40u: handler = subtitle_duration; break;
        case 0x0069BE60u: handler = unknown_subtitle_parse_position; break;
        case 0x0069BEF0u: handler = unknown_subtitle_parse_color; break;
        case 0x0069BF70u: handler = subtitle_text; break;
        }
        unsigned short const *token = (unsigned short const *)ra2port::original_data_pointer(entry[0]);
        if (!handler || !token) {
            fprintf(stderr, "subtitles: keyword %d (0x%08X, handler 0x%08X) not mapped\n", i, entry[0], entry[1]);
            continue;
        }
        unknown_subtitle_hooks_7f62f0[hooks].Token = token;
        unknown_subtitle_hooks_7f62f0[hooks].Handler = handler;
        ++hooks;
    }
    int names = 0;
    for (int i = 0; i < 7; ++i) {
        uint32_t entry[2];
        if (!ra2port::original_bytes(0x007F6320u + 8u * (uint32_t)i, entry, sizeof entry) || entry[0] == 0) break;
        unsigned short const *name = (unsigned short const *)ra2port::original_data_pointer(entry[0]);
        if (!name) continue;
        unknown_subtitle_alignments_7f6320[names].Name = name;
        unknown_subtitle_alignments_7f6320[names].Alignment = (int)entry[1];
        ++names;
    }
    fprintf(stderr, "subtitles: %d keywords and %d alignment names from game.exe\n", hooks, names);
}
