// Behaviour rows (Scout1125, native M15; NOT EXACT, native/behaviour_rewrites.json):
// the depth read/write blitters for 16-bit surfaces that the win run reaches
// (a darkened, depth-written shape once the harness builds a barracks; the 25%
// and 75% translucent siblings of the same family). Written from the recovered
// siblings: the depth test and wrap of BlitTransLucent50ZReadWrite<unsigned short>
// 0x00487C50 (depth below the buffer: draw and store the depth), the darken
// pixel of BlitTransDarkenZRead 0x00487390 and the 25%/75% pixels of
// BlitTransLucent25ZRead/75ZRead; each backward blit runs the forward one
// (the TibSun archive's blitblit.h: "the backward moving method ... in terms of
// the forward copying method").
#include "shared/basic_depth_write_blitters.hpp"
#include "shared/blit.hpp"

namespace {
template <class Pixel>
void Depth_Write_Blit(void *target, void const *source, int length, int depth, void *depth_buffer, Pixel pixel)
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    for (int index = 0; index < length; index++) {
        if (depth < *depth_ptr) {
            unsigned char color = *(unsigned char const *)source;
            if (color) {
                *(unsigned short *)target = pixel(*(unsigned short *)target, color);
                *depth_ptr = (unsigned short)depth;
            }
        }
        ++depth_ptr;
        source = (unsigned char const *)source + 1;
        target = (unsigned short *)target + 1;
        depth_ptr = ABufferPtr->Wrap_Pointer(depth_ptr);
    }
}
}  // namespace

template <>
void BlitTransDarkenZReadWrite<unsigned short>::BlitForward(void *target, void const *source, int length, int depth,
                                                             void *depth_buffer, void *, int, int) const
{
    unsigned short mask = ChannelMask;
    Depth_Write_Blit(target, source, length, depth, depth_buffer,
                     [mask](unsigned short dest, unsigned char) { return (unsigned short)((dest >> 1) & mask); });
}

template <>
void BlitTransDarkenZReadWrite<unsigned short>::BlitBackward(void *target, void const *source, int length, int depth,
                                                              void *depth_buffer, void *alpha_buffer, int alpha_level) const
{
    BlitForward(target, source, length, depth, depth_buffer, alpha_buffer, alpha_level, 0);
}

template <>
void BlitTransLucent25ZReadWrite<unsigned short>::BlitForward(void *target, void const *source, int length, int depth,
                                                               void *depth_buffer, void *, int, int) const
{
    unsigned short mask = ChannelMask;
    unsigned short const *palette = Palette;
    Depth_Write_Blit(target, source, length, depth, depth_buffer, [mask, palette](unsigned short dest, unsigned char color) {
        return (unsigned short)(((palette[color] >> 2) & mask) * 3 + ((dest >> 2) & mask));
    });
}

template <>
void BlitTransLucent25ZReadWrite<unsigned short>::BlitBackward(void *target, void const *source, int length, int depth,
                                                                void *depth_buffer, void *alpha_buffer, int alpha_level) const
{
    BlitForward(target, source, length, depth, depth_buffer, alpha_buffer, alpha_level, 0);
}

template <>
void BlitTransLucent75ZReadWrite<unsigned short>::BlitForward(void *target, void const *source, int length, int depth,
                                                               void *depth_buffer, void *, int, int) const
{
    unsigned short mask = ChannelMask;
    unsigned short const *palette = Palette;
    Depth_Write_Blit(target, source, length, depth, depth_buffer, [mask, palette](unsigned short dest, unsigned char color) {
        return (unsigned short)(((palette[color] >> 2) & mask) + ((dest >> 2) & mask) * 3);
    });
}

template <>
void BlitTransLucent75ZReadWrite<unsigned short>::BlitBackward(void *target, void const *source, int length, int depth,
                                                                void *depth_buffer, void *alpha_buffer, int alpha_level) const
{
    BlitForward(target, source, length, depth, depth_buffer, alpha_buffer, alpha_level, 0);
}

template <>
void BlitTransLucent50ZReadWrite<unsigned short>::BlitBackward(void *target, void const *source, int length, int depth,
                                                                void *depth_buffer, void *alpha_buffer, int alpha_level) const
{
    BlitForward(target, source, length, depth, depth_buffer, alpha_buffer, alpha_level, 0);
}
