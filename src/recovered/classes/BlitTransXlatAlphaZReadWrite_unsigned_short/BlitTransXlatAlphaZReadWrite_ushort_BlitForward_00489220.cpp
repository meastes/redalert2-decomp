// Depth-test each source pixel before reading its index and translating through the alpha row.
#include "shared/trans_xlat_alpha_depth_blitter.hpp"
#include "shared/blit.hpp"
// @ref ?AlphaBuffer@@3PAVZBuffer@@A=008315b4
// @ref ?DepthBuffer@@3PAVABuffer@@A=00839c6c
template <>
void BlitTransXlatAlphaZReadWrite<unsigned short>::BlitForward(void *target, void const *source, int length,
                                                        int depth, void *depth_buffer, void *buffer, int level, int) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned short *alpha = (unsigned short *)buffer;
    int row = level > 0 ? level : 0;
    row = (row * 261) >> 11;
    if (row > 254) row = 254;
    unsigned short const *table = AlphaTable[row];
    if (length > 0) {
        int count = length;
        unsigned short *dest = (unsigned short *)target;
        do {
            unsigned short value = *depth_ptr;
            if (depth < value) {
                unsigned char color = *(unsigned char const *)source;
                if (color) {
                    unsigned int translated = table[*alpha];
                    *dest = Palette[translated | color];
                    *depth_ptr = (unsigned short)depth;
                }
            }
            source = (unsigned char const *)source + 1;
            dest++;
            depth_ptr++;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
            alpha++;
            alpha = AlphaBuffer->Wrap_Pointer(alpha);
        } while (--count);
    }
}
