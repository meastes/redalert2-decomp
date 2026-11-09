// Behaviour rows (Scout1111, native M12; NOT EXACT, native/behaviour_rewrites.json):
// the 50% translucent blitter for 16-bit surfaces, which the sidebar's build clock
// (SidebarClass::StripClass 0x0067F800, flag 0x404) reaches once the harness
// builds, and the backward blits of the three translucent blitters. Written from
// the adopted TibSun archive text (blitblit.h BlitTransLucent50: half of the
// destination plus half of the translated source, masked per channel; "the
// backward moving method ... in terms of the forward copying method") and the
// recovered 25%/75% forward bodies (0x00487110, 0x00486FF0).
#include "shared/basic_lucent_blitters.hpp"

template <>
void BlitTransLucent50<unsigned short>::BlitForward(void *target, void const *source, int length,
                                                    int, void *, void *, int, int) const
{
    for (int index = 0; index < length; index++) {
        unsigned char color = *(unsigned char const *)source;
        source = (unsigned char const *)source + 1;
        if (color) {
            *(unsigned short *)target = (unsigned short)(((*(unsigned short *)target >> 1) & ChannelMask)
                                                         + ((Palette[color] >> 1) & ChannelMask));
        }
        target = (unsigned short *)target + 1;
    }
}

template <>
void BlitTransLucent50<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                     int z_min, void *z_buff, void *a_buff, int alpha_level) const
{
    BlitForward(target, source, length, z_min, z_buff, a_buff, alpha_level, 0);
}

template <>
void BlitTransLucent25<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                     int z_min, void *z_buff, void *a_buff, int alpha_level) const
{
    BlitForward(target, source, length, z_min, z_buff, a_buff, alpha_level, 0);
}

template <>
void BlitTransLucent75<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                     int z_min, void *z_buff, void *a_buff, int alpha_level) const
{
    BlitForward(target, source, length, z_min, z_buff, a_buff, alpha_level, 0);
}
