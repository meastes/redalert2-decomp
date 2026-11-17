// unvqtblc.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/unvqtblc.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unvq_table.hpp"


// bool __fastcall Hicolor_Init_Table(int mode) -- 0x0070B690
// Allocates the 32x32x32 pixel table and fills it for a screen pixel format.

static bool Hicolor_Set_Mode(int mode);

bool __fastcall Hicolor_Init_Table(int mode)
{
    Hicolor_Clear_Table();
    HicolorTable = new unsigned short[65534];
    if (HicolorTable != 0) {
        return Hicolor_Set_Mode(mode);
    }
    return false;
}


// void Hicolor_Clear_Table() -- 0x0070B7D0
// Releases the pixel table.

void Hicolor_Clear_Table()
{
    if (HicolorTable != 0) {
        delete [] HicolorTable;
        HicolorTable = 0;
    }
}

static bool Hicolor_Set_Mode(int mode)
{
    bool valid = false;
    int red_shift;
    int green_shift;
    int blue_shift;

    switch (mode) {
        case HICOLOR_MODE_555:
            red_shift = 10;
            green_shift = 5;
            blue_shift = 0;
            valid = true;
            break;
        case HICOLOR_MODE_655:
            red_shift = 11;
            green_shift = 5;
            blue_shift = 0;
            valid = true;
            break;
        case HICOLOR_MODE_565:
            red_shift = 11;
            green_shift = 6;
            blue_shift = 0;
            valid = true;
            break;
        case HICOLOR_MODE_556:
            red_shift = 11;
            green_shift = 6;
            blue_shift = 1;
            valid = true;
            break;
        default:
            break;
    }

    if (valid) {
        for (int red = 0; red < 32; red++) {
            for (int green = 0; green < 32; green++) {
                for (int blue = 0; blue < 32; blue++) {
                    HicolorTable[(unsigned short)(((red << 5) | green) << 5) | (unsigned short)blue] =
                        (unsigned short)((red << red_shift) | (green << green_shift) | (blue << blue_shift));
                }
            }
        }
        return true;
    }
    return false;
}


// void __fastcall Hicolor_Translate(void *buffer, int count) -- 0x0070B7F0
// Converts count 15-bit pixels in place through the pixel table.

void __fastcall Hicolor_Translate(void *buffer, int count)
{
    unsigned short *pixel = (unsigned short *)buffer;
    while (count > 0) {
        *pixel = HicolorTable[*pixel];
        pixel++;
        count--;
    }
}


// Table-driven hi-colour VQA decoder, complete original 0x0070B820.
// Uses the existing six-argument cdecl API; movie callback aliases stay unchanged.

// The real reference helper: fills a sequence of 32-bit pixel pairs.
static inline void Fill_Pixel_Pairs(void *destination, unsigned int value, unsigned int count)
{
    unsigned int *pairs = (unsigned int *)destination;
    for (unsigned int index = 0; index < count; ++index) pairs[index] = value;
}

void __cdecl UnVQ2_4x4_Table(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer,
    unsigned long blocks_per_row, unsigned long rows, unsigned long width)
{
    width *= 2;
    unsigned char *end = buffer + rows * width * 4;
    unsigned char *row_start = buffer;
    unsigned char *row_end = buffer + blocks_per_row * 8;
    unsigned int block_height = width * 4;
    unsigned short *commands = (unsigned short *)pointers;
    unsigned char *out = buffer;
    while (out < end) {
        unsigned short token = *commands++;
        unsigned int value = token;
        unsigned int count = value & 0xffff0fff;
        int opcode = value & 0xf000;
        if (opcode <= 0x3000) {
            if (opcode != 0x3000) {
                if (opcode) {
                    if (opcode != 0x1000) {
                        if (opcode == 0x2000) {
                            unsigned short *block = (unsigned short *)(codebook + *commands++ * 32);
                            unsigned int line_count = 4;
                            while (line_count > 0) {
                                unsigned int low = block[0];
                                unsigned int high = block[1];
                                unsigned int first_pair = low | (high << 16);
                                block += 2;
                                unsigned char *pixel = out;
                                unsigned int repeats = count;
                                while (repeats > 0) {
                                    *(unsigned int *)pixel = first_pair;
                                    pixel += 8;
                                    --repeats;
                                }
                                low = block[0];
                                high = block[1];
                                unsigned int second_pair = low | (high << 16);
                                block += 2;
                                unsigned char *second_pixel = out + 4;
                                unsigned int second_repeats = count;
                                while (second_repeats > 0) {
                                    *(unsigned int *)second_pixel = second_pair;
                                    second_pixel += 8;
                                    --second_repeats;
                                }
                                out += width;
                                --line_count;
                            }
                            out += count * 8 - block_height;
                        }
                    } else {
                        out += count * 8;
                    }
                } else {
                    unsigned int length = 2 * count;
                    unsigned short color = HicolorTable[*commands++];
                    unsigned int pair = color | (color << 16);
                    Fill_Pixel_Pairs(out, pair, length);
                    Fill_Pixel_Pairs(out + width, pair, length);
                    Fill_Pixel_Pairs(out + 2 * width, pair, length);
                    Fill_Pixel_Pairs(out + 2 * width + width, pair, length);
                    out += count * 8;
                }
            } else {
                unsigned short *block = (unsigned short *)(codebook + *commands++ * 32);
                unsigned int line_count = 4;
                while (line_count > 0) {
                    unsigned short *pixel = (unsigned short *)out;
                    unsigned int columns = 4;
                    while (columns > 0) {
                        unsigned short color = *block;
                        if (!(color & 0x8000)) {
                            unsigned short *repeat_pixel = pixel;
                            unsigned int repeats = count;
                            while (repeats > 0) {
                                *repeat_pixel = color;
                                repeat_pixel += 4;
                                --repeats;
                            }
                        }
                        ++pixel;
                        ++block;
                        --columns;
                    }
                    out += width;
                    --line_count;
                }
                out += 8 * count - block_height;
            }
        } else {
            switch (opcode) {
            case 0x5000:
                if (count > 0) {
                    int pitch = 4 * (width >> 2);
                    while (count > 0) {
                        unsigned short color = HicolorTable[*commands];
                        unsigned int pair = color | (color << 16);
                        ++commands;
                        unsigned char *line = out;
                        unsigned int line_count = 4;
                        while (line_count > 0) {
                            *(unsigned int *)line = pair;
                            ((unsigned int *)line)[1] = pair;
                            line += pitch;
                            --line_count;
                        }
                        out += 8;
                        --count;
                    }
                }
                break;
            case 0x6000:
                if (count > 0) {
                    int pitch = 4 * (width >> 2);
                    while (count > 0) {
                        unsigned short *block = (unsigned short *)(codebook + *commands++ * 32);
                        unsigned char *line = out;
                        unsigned int line_count = 4;
                        while (line_count > 0) {
                            unsigned int low = block[0];
                            unsigned int high = block[1];
                            unsigned int first_pair = low | (high << 16);
                            block += 2;
                            *(unsigned int *)line = first_pair;
                            low = block[0];
                            high = block[1];
                            unsigned int second_pair = low | (high << 16);
                            block += 2;
                            ((unsigned int *)line)[1] = second_pair;
                            line += pitch;
                            --line_count;
                        }
                        out += 8;
                        --count;
                    }
                }
                break;
            case 0x7000:
                if (count > 0) {
                    while (count > 0) {
                        unsigned short *block = (unsigned short *)(codebook + *commands++ * 32);
                        unsigned char *line = out;
                        int line_count = 4;
                        while (line_count) {
                            unsigned short *pixel = (unsigned short *)line;
                            unsigned int columns = 4;
                            while (columns > 0) {
                                if (!(*block & 0x8000)) *pixel = *block;
                                ++pixel;
                                ++block;
                                --columns;
                            }
                            --line_count;
                            line += 2 * (width >> 1);
                        }
                        out += 8;
                        --count;
                    }
                }
                break;
            }
        }
        if (out == row_end) {
            row_start += block_height;
            out = row_start;
            row_end = out + blocks_per_row * 8;
        }
    }
}


// void __cdecl UnVQ1_4x4_Table(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer,
//     unsigned long blocks_per_row, unsigned long rows, unsigned long width) -- 0x0070BF60
// Purpose: the hi-colour 4x4 block decoder for half-height (interlaced) frames:
// each block is drawn from every other row of its codebook entry into every
// other row of the buffer; commands 0x0000 draw a block, 0x2000 draw it with
// 0x8000 as transparent, 0x4000 skip a block.

void __cdecl UnVQ1_4x4_Table(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer,
    unsigned long blocks_per_row, unsigned long rows, unsigned long width)
{
    width *= 2;
    unsigned int block_row_stride = width * 4;

    unsigned char *end = buffer + rows * width * 4;
    unsigned char *dst = buffer;

    unsigned char *row_base = buffer;
    unsigned char *row_end = row_base + blocks_per_row * 8;

    unsigned short *src = (unsigned short *)pointers;

    while (dst < end) {
        unsigned int command = (*src & 0xE000);
        unsigned short cb_index = (*src & 0x1FFF);

        src += 1;

        switch (command) {
        case 0x0000: {
            unsigned char const *cb = codebook + (cb_index << 5);

            for (unsigned int r = 0; r < 2; r++) {
                unsigned int lo;
                unsigned int hi;

                lo = ((unsigned int)*(unsigned short const *)(cb + 2) << 16) | *(unsigned short const *)(cb + 0);
                cb += 4;
                *(unsigned int *)(dst + 0) = lo;

                hi = ((unsigned int)*(unsigned short const *)(cb + 2) << 16) | *(unsigned short const *)(cb + 0);
                cb += 12;
                *(unsigned int *)(dst + 4) = hi;

                dst += width * 2;
            }

            dst += 8 - block_row_stride;
        } break;

        case 0x2000: {
            unsigned short const *cb = (unsigned short const *)(codebook + (cb_index << 5));

            for (unsigned int r = 0; r < 2; r++) {
                unsigned short *p = (unsigned short *)dst;

                for (unsigned int c = 0; c < 4; c++) {
                    if (!(*cb & 0x8000)) {
                        *p = *cb;
                    }
                    p++;
                    cb++;
                }
                cb += 4;
                dst += width * 2;
            }

            dst += 8 - block_row_stride;
        } break;

        case 0x4000: {
            dst += 8;
        } break;

        default:
            break;
        }

        if (dst == row_end) {
            row_base += block_row_stride;
            dst = row_base;
            row_end = row_base + blocks_per_row * 8;
        }
    }
}

// Not exact (RA2_NON_MATCHING): UnVQ1_4x2_Table at 0x0070C0A0, best score 75.55, 2026-11-13; differs: 294 instructions in the original, 299 in the draft (shape 95.45)
#ifdef RA2_NON_MATCHING
// void __cdecl UnVQ1_4x2_Table(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer,
//     unsigned long blocks_per_row, unsigned long rows, unsigned long width) -- 0x0070C0A0
// Half-height (interlaced) form of UnVQ1_4x4_Table: two lines per block, drawn from every
// other line of the 32-byte codebook entry into every other line of the buffer.

void __cdecl UnVQ1_4x2_Table(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer,
    unsigned long blocks_per_row, unsigned long rows, unsigned long width)
{
    width *= 2;
    unsigned int block_height = width * 4;
    unsigned char *end = buffer + (rows * width * 4);
    unsigned char *out = buffer;
    unsigned char *row_start = buffer;
    unsigned char *row_end = row_start + blocks_per_row * 8;
    unsigned short *src = (unsigned short *)pointers;

    unsigned int repeats;
    unsigned int code;
    unsigned int block_index;

    while (out < end) {
        unsigned int opcode = (*src & 0xe000);
        unsigned short index = (*src & 0x1fff);
        src++;

        if (opcode <= 0x6000) {
            if (opcode != 0x6000) {
                if (opcode) {
                    if (opcode != 0x2000) {
                        if (opcode == 0x4000) {
                            unsigned int count = ((index >> 7) & 0x3e) + 2;
                            const unsigned char *block = codebook + (*((unsigned char *)src - 2) << 5);
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
                            unsigned char *bytes = (unsigned char *)src;
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
                            src = (unsigned short *)bytes;
                        }
                    } else {
                        code = index;
                        code &= 0xffff;
                        repeats = ((code >> 7) & 0x3e) + 2;
                        block_index = code & 0xff;
                    check_count:
                        if (repeats == 0) {
                            repeats = *(unsigned char *)src;
                            block_index = index;
                            src = (unsigned short *)((unsigned char *)src + 1);
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
                unsigned int count = *(unsigned char *)src;
                src = (unsigned short *)((unsigned char *)src + 1);
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
#endif

