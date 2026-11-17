// Test scaffolding (lane C17; NOT game behaviour): RA2_NATIVE_DRAW_SELFTEST=1 makes main run
// this before WinMain and exit with its result (ctest native-draw-selftest, also under Wine).
// One case per routine family where a pointer is moved by a negative offset held in unsigned
// 32-bit arithmetic (VC6's 32-bit pointers wrap it back; LP64/LLP64 zero-extended it: the
// lane C17 pitch sweep, port/fixups.json "signed, as VC6's 32-bit wrap").
//
// Text: BitFontClass::Draw_Char 0x004318E0 with a character that starts left of the surface on
// its top row (codex4 R740/R749/R751: SAVE0029's team-number label at the top left). The row
// pointer Buffer + (Pitch * y + x) has an unsigned Pitch: VC6's 32-bit pointers wrap the
// negative offset back into the surface, LP64/LLP64 zero-extended it to about 8 GB past it
// (port/fixups.json computes it signed). A synthetic 8x4 glyph, every bit set, is drawn at
// -4,0 and at 4,-2 on an 8x4 memory surface; the expected pixels are checked.
#include "shared/text_print.hpp"
#include "shared/surface.hpp"
#include "shared/unvq_table.hpp"

extern "C" int printf(char const *format, ...);

static int font_cases()
{
    static unsigned short map[0x10000];
    static unsigned char glyph[1 + 4];
    for (int i = 0; i < (int)sizeof glyph; ++i) glyph[i] = 0xFF;
    glyph[0] = 8;                                    // the width byte, then one byte per row
    map['A'] = 1;
    static FontDataType data;
    data.Width = 1;
    data.unknown_08 = 4;
    data.GlyphCount = 1;
    data.GlyphSize = sizeof glyph;
    data.Map = map;
    data.Glyphs = glyph;
    alignas(BitFontClass) static unsigned char storage[sizeof(BitFontClass)];
    BitFontClass *font = (BitFontClass *)storage;    // only the fields Draw_Char reads; no font file
    font->Data = &data;
    font->Color = 0x1234;
    font->Clipping = 1;
    static unsigned short pixels[8 * 4];
    BSurface surface(8, 4, 2, pixels);
    int failures = 0;
    struct Case { int x, y; } cases[] = {{-4, 0}, {4, -2}};
    for (Case const &c : cases) {
        for (int i = 0; i < 8 * 4; ++i) pixels[i] = 0;
        tagRECT box = {0, 0, 7, 3};
        font->Set_Clip_Box(&box);
        font->Set_Surface(&surface);
        font->Draw_Char('A', c.x, c.y, -1);
        font->Release_Surface(&surface);
        for (int y = 0; y < 4; ++y) {
            for (int x = 0; x < 8; ++x) {
                bool inside = x >= c.x && x < c.x + 8 && y >= c.y && y < c.y + 4;
                if (pixels[y * 8 + x] != (inside ? 0x1234 : 0)) {
                    printf("draw-selftest: text: character at %d,%d: pixel %d,%d is %04x\n", c.x, c.y, x, y, pixels[y * 8 + x]);
                    ++failures;
                }
            }
        }
    }
    printf("draw-selftest: text: %s\n", failures ? "FAILED" : "passed");
    return failures;
}

// VQA: UnVQ2_4x4_Table 0x0070BB30 (unvqtblc.cpp; the C decoders of vqalib_unvq.cpp step the same
// way). After a block's four rows the output steps back to the next block's top row by
// count * 8 - block_height, a negative unsigned value: natively the second block landed about
// 4 GB away (past the end, so it was simply not drawn). Two 4x4 blocks side by side, codebook
// entries 0x1111 and 0x2222, on an 8x4 16-bit buffer.
static int vqa_cases()
{
    static unsigned short codebook[2 * 16];
    for (int i = 0; i < 16; ++i) {
        codebook[i] = 0x1111;
        codebook[16 + i] = 0x2222;
    }
    static unsigned short commands[] = {0x2001, 0, 0x2001, 1, 0, 0, 0, 0};
    static unsigned short pixels[8 * 4];
    for (int i = 0; i < 8 * 4; ++i) pixels[i] = 0;
    UnVQ2_4x4_Table((unsigned char *)codebook, (unsigned char *)commands, (unsigned char *)pixels, 2, 1, 8);
    int failures = 0;
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 8; ++x) {
            unsigned short want = x < 4 ? 0x1111 : 0x2222;
            if (pixels[y * 8 + x] != want) {
                if (failures < 4) printf("draw-selftest: vqa: pixel %d,%d is %04x, want %04x\n", x, y, pixels[y * 8 + x], want);
                ++failures;
            }
        }
    }
    printf("draw-selftest: vqa: %s\n", failures ? "FAILED" : "passed");
    return failures;
}

extern "C" int ra2_native_draw_selftest(void)
{
    int failures = font_cases() + vqa_cases();
    printf("draw-selftest: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
