// void __cdecl UnVQ1_4x4_Table(unsigned char *codebook, unsigned char *pointers, unsigned char *buffer,
//     unsigned long blocks_per_row, unsigned long rows, unsigned long width) -- 0x0070BF60
// Purpose: the hi-colour 4x4 block decoder for half-height (interlaced) frames:
// each block is drawn from every other row of its codebook entry into every
// other row of the buffer; commands 0x0000 draw a block, 0x2000 draw it with
// 0x8000 as transparent, 0x4000 skip a block.
#include "shared/unvq_table.hpp"

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
