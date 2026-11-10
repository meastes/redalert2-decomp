// Depth-test each source pixel before reading its index and translating through the alpha row.
#include "shared/trans_xlat_alpha_read_blitter.hpp"
#include "shared/blit.hpp"
// @ref ?AlphaBuffer@@3PAVZBuffer@@A=008315b4
// @ref ?DepthBuffer@@3PAVABuffer@@A=00839c6c
template <>
void BlitTransLucent25AlphaZRead<unsigned short>::BlitForward(void *target, void const *source, int length,
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
        do {
            if (depth < *depth_ptr++) {
                unsigned char color = *(unsigned char const *)source;
                if (color) {
                    unsigned int translated = table[*alpha];
                    *(unsigned short *)target = ((Palette[translated | color] >> 2) & ChannelMask) * 3 + ((*(unsigned short *)target >> 2) & ChannelMask);
                }
            }
            source = (unsigned char const *)source + 1;
            target = (unsigned short *)target + 1;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
            alpha++;
            alpha = AlphaBuffer->Wrap_Pointer(alpha);
        } while (--count);
    }
}
