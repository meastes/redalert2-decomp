// Behaviour rows for native M22 (Scout1177): the backward entries (slot 2) of the
// blitters whose vtables are live. Every backward entry the original keeps for these
// families forwards to its own forward slot with 0 as the last argument (0x00486210
// BlitTransXlat<unsigned short>, 0x00487570 BlitTransLucent50ZRead, blitter.hpp's
// BlitTrans): a translation reads 8-bit shape data and writes another surface, so
// the two never overlap and the copy direction does not matter. NOT EXACT.
#include "shared/blitter.hpp"
#include "shared/basic_xlat_blitters.hpp"
#include "shared/destination_blitters.hpp"
#include "shared/basic_depth_blitters.hpp"
#include "shared/basic_depth_write_blitters.hpp"
#include "shared/translucent_write_alpha_blitter.hpp"
#include "shared/basic_depth_warp_blitters.hpp"
#include "shared/conditional_alpha_blitters.hpp"
template <>
void BlitPlainXlat<unsigned char>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitPlainXlat<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitPlainXlatZRead<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitPlainXlatZReadWrite<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTransDarken<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTransDarkenZRead<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTransLucent25ZRead<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTransLucent25ZReadWarp<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTransLucent50ZReadWarp<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTransLucent75ZRead<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTransLucent75ZReadWarp<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTransRemapDest<unsigned char>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTransRemapXlat<unsigned char>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTransXlat<unsigned char>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTransXlat<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTransXlatMultWriteAlpha<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTransXlatZRead<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTransXlatZReadWrite<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTransZRemapXlat<unsigned char>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTransZRemapXlat<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTransZRemapXlatZRead<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTransZRemapXlatZReadWrite<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTranslucent50NonzeroAlpha<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTranslucent50ZeroAlpha<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}
template <>
void BlitTranslucentWriteAlpha<unsigned short>::BlitBackward(void *target, void const *source, int length, int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}

// 0x00775980 BlitTrans<unsigned char>::BlitForward (handwritten assembly in the
// original): copies every source byte but colour 0, as blitter.hpp's template does
// for the 16-bit copy.
template <>
void BlitTrans<unsigned char>::BlitForward(void *target, void const *source, int length,
                                           int, void *, void *, int, int) const
{
    for (int index = 0; index < length; index++) {
        unsigned char color = ((unsigned char const *)source)[index];
        if (color != 0) {
            ((unsigned char *)target)[index] = color;
        }
    }
}
