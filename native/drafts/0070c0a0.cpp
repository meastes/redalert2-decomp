// NON-EXACT DRAFT 0x70C0A0 FUN_0070c0a0
// entry 0x70C0A0; exact-match score 75.95 (misses_index best); plan "review: rewrite likely"
// origin lane: Scout845 (build/class-work/Scout845/w/u21.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// void __cdecl UnVQ1_4x2_Table(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer,
//     unsigned long blocks_per_row, unsigned long rows, unsigned long width) -- 0x0070C0A0
// Half-height (interlaced) form of UnVQ1_4x4_Table: two lines per block, drawn from every
// other line of the 32-byte codebook entry into every other line of the buffer.
#include "shared/unvq_table.hpp"

void __cdecl UnVQ1_4x2_Table(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer,
    unsigned long blocks_per_row, unsigned long rows, unsigned long width)
{
    width *= 2;
    unsigned int block_height = width * 4;
    unsigned char *end = buffer + (rows * width * 4);
    unsigned char *out = buffer;
    unsigned char *row_start = buffer;
    unsigned char *row_end = row_start + blocks_per_row * 8;
    unsigned char *commands = pointers;

    unsigned int repeats;
    unsigned int code;
    unsigned int block_index;

    while (out < end) {
        unsigned int opcode = (*(unsigned short *)commands & 0xe000);
        unsigned short index = (*(unsigned short *)commands & 0x1fff);
        commands += 2;

        if (opcode <= 0x6000) {
            if (opcode != 0x6000) {
                if (opcode) {
                    if (opcode != 0x2000) {
                        if (opcode == 0x4000) {
                            unsigned int count = ((index >> 7) & 0x3e) + 2;
                            const unsigned char *block = codebook + (*(commands - 2) << 5);
                            unsigned char *pixel = out;
                            for (unsigned int line_count = 0; line_count < 2; ++line_count) {
                                unsigned int low = (*(unsigned short *)(block + 2) << 16) | *(unsigned short *)(block + 0);
                                block += 4;
                                *(unsigned int *)(pixel + 0) = low;
                                unsigned int high = (*(unsigned short *)(block + 2) << 16) | *(unsigned short *)(block + 0);
                                block += 12;
                                *(unsigned int *)(pixel + 4) = high;
                                pixel += 4 * (width >> 2);
                            }
                            unsigned char *bytes = commands;
                            out += 8;
                            for (int i = 0; i < count; i++) {
                                unsigned int next_index = *bytes;
                                bytes += 1;
                                unsigned char *next_pixel = out;
                                const unsigned char *next_block = codebook + (next_index << 5);
                                for (unsigned int line_count = 0; line_count < 2; ++line_count) {
                                    unsigned int low = (*(unsigned short *)(next_block + 2) << 16) | *(unsigned short *)(next_block + 0);
                                    next_block += 4;
                                    *(unsigned int *)(next_pixel + 0) = low;
                                    unsigned int high = (*(unsigned short *)(next_block + 2) << 16) | *(unsigned short *)(next_block + 0);
                                    next_block += 12;
                                    *(unsigned int *)(next_pixel + 4) = high;
                                    next_pixel += 8 * (width >> 2);
                                }
                                out += 8;
                            }
                            commands = bytes;
                        }
                    } else {
                        code = index;
                        code &= 0xffff;
                        repeats = ((code >> 7) & 0x3e) + 2;
                        block_index = code & 0xff;
                    check_count:
                        if (repeats == 0) {
                            repeats = *commands;
                            block_index = index;
                            commands += 1;
                        }
                        {
                        unsigned int line_count = 2;
                        unsigned short *block = (unsigned short *)&codebook[32 * block_index];
                        do {
                            unsigned int low = block[0];
                            unsigned int high = block[1];
                            unsigned int pair = low | (high << 16);
                            block += 2;
                            unsigned char *pixel = out;
                            for (unsigned int i = 0; i < repeats; i++) {
                                *(unsigned int *)pixel = pair;
                                pixel += 8;
                            }
                            low = block[0];
                            high = block[1];
                            pair = low | (high << 16);
                            block += 6;
                            unsigned char *second_pixel = out + 4;
                            for (unsigned int j = 0; j < repeats; j++) {
                                *(unsigned int *)second_pixel = pair;
                                second_pixel += 8;
                            }
                            out += 2 * width;
                            --line_count;
                        } while (line_count);
                        out += 8 * repeats - (int)(width << 2);
                    }
                    }
                } else {
                    out += 8 * (unsigned char)index;
                }
            } else {
                const unsigned char *block = codebook + ((index & 0xffff) << 5);
                for (unsigned int line = 0; line < 4; ++line) {
                    unsigned int low = (*(unsigned short *)(block + 2) << 16) | *(unsigned short *)(block + 0);
                    block += 4;
                    *(unsigned int *)(out + 0) = low;
                    unsigned int high = (*(unsigned short *)(block + 2) << 16) | *(unsigned short *)(block + 0);
                    block += 12;
                    *(unsigned int *)(out + 4) = high;
                    out += width * 2;
                }
                out += 8 - (int)(width << 2);
            }
        } else {
            switch (opcode) {
            case 0x8000: {
                unsigned short *block = (unsigned short *)&codebook[32 * index];
                int line_count = 4;
                do {
                    unsigned short *pixel = (unsigned short *)out;
                    int columns = 4;
                    do {
                        if ((*block & 0x8000) == 0) {
                            *pixel = *block;
                        }
                        pixel++;
                        block++;
                        --columns;
                    } while (columns);
                    block += 4;
                    out += 2 * width;
                    --line_count;
                } while (line_count);
                out += 8 - (int)(width << 2);
                break;
            }

            case 0xa000:
                repeats = 0;
                goto check_count;

            case 0xc000: {
                unsigned int count = *commands;
                commands += 1;
                unsigned short *block = (unsigned short *)&codebook[32 * index];
                for (unsigned int line_count = 0; line_count < 2; line_count++) {
                    unsigned short *pixel = (unsigned short *)out;
                    for (unsigned int columns = 0; columns < 4; columns++) {
                        unsigned short color = *block;
                        if ((*block & 0x8000) == 0) {
                            unsigned short *repeat_pixel = pixel;
                            for (unsigned int k = 0; k < count; k++) {
                                *repeat_pixel = color;
                                repeat_pixel += 4;
                            }
                        }
                        pixel++;
                        block++;
                    }
                    block += 4;
                    out += 2 * width;
                }
                out += 8 * count - (int)(width << 2);
                break;
            }
            }
        }
        if (out == row_end) {
            row_start += block_height;
            out = row_start;
            row_end = row_start + blocks_per_row * 8;
        }
    }
}
