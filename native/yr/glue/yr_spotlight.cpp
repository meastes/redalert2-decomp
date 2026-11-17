// NON-EXACT NATIVE BEHAVIOUR (lane Y12): the spotlight's brightening blitters, gamemd.exe
// 0x007DEEFA..0x007DF640 (hand-written assembly the YR tree declares extern "C" in ovrlight.cpp and
// SpotLightClass::Draw calls per colour mode; not in the tree, a link-closure trap reached by the
// first spotlight drawn in a skirmish). Written from gamemd.exe's code: for every non-zero
// intensity s of the 256-wide mask, each channel of the 16-bit pixel below, expanded to 8 bits,
// becomes c + (c * s >> 8) saturated at 255, and is packed back (the tree's C fallback's
// MIN(255, c * (s + 256) >> 8) for the same pixel). The MMX variants (0x007DF280, 0x007DF3C2,
// 0x007DF500, 0x007DF640) take the colour table SpotLightClass::Init builds (RGB888 per 16-bit
// colour) and produce the same pixels; here they use the same per-channel code.
// Behaviour code for the native build; never part of an exact build.
#include <stdint.h>

namespace {

// Each channel: its width in bits and its position in the 16-bit pixel.
template <int RBits, int RPos, int GBits, int GPos, int BBits, int BPos>
void Spot_Brighten(unsigned char * mul_buffer, unsigned short * color_buffer, int mulbuff_width, int color_buff_width,
	int width, int height)
{
	for (int y = 0; y < height; ++y) {
		unsigned char * s = mul_buffer;
		unsigned short * d = color_buffer;
		for (int x = 0; x < width; ++x, ++s, ++d) {
			unsigned scale = *s;
			if (scale == 0) continue;
			unsigned p = *d;
			unsigned c[3];
			int const bits[3] = {RBits, GBits, BBits};
			int const pos[3] = {RPos, GPos, BPos};
			unsigned out = 0;
			for (int k = 0; k < 3; ++k) {
				c[k] = ((p >> pos[k]) & ((1u << bits[k]) - 1)) << (8 - bits[k]);
				unsigned v = c[k] + ((c[k] * scale) >> 8);
				if (v > 255) v = 255;
				out |= (v >> (8 - bits[k])) << pos[k];
			}
			*d = (unsigned short)out;
		}
		mul_buffer += mulbuff_width;
		color_buffer = (unsigned short *)((unsigned char *)color_buffer + color_buff_width);
	}
}

} // namespace

extern "C" {

// 5:6:5 (0x007DEEFA), 6:5:5 (0x007DEFD7), 5:5:6 (0x007DF0B4), 5:5:5 (0x007DF191)
void FUN_007DEEFA(unsigned char * m, unsigned short * c, int mw, int cw, int w, int h) { Spot_Brighten<5, 11, 6, 5, 5, 0>(m, c, mw, cw, w, h); }
void FUN_007DEFD7(unsigned char * m, unsigned short * c, int mw, int cw, int w, int h) { Spot_Brighten<6, 10, 5, 5, 5, 0>(m, c, mw, cw, w, h); }
void FUN_007DF0B4(unsigned char * m, unsigned short * c, int mw, int cw, int w, int h) { Spot_Brighten<5, 11, 5, 6, 6, 0>(m, c, mw, cw, w, h); }
void FUN_007DF191(unsigned char * m, unsigned short * c, int mw, int cw, int w, int h) { Spot_Brighten<5, 10, 5, 5, 5, 0>(m, c, mw, cw, w, h); }

// The MMX variants: 5:6:5 (0x007DF280), 5:5:5 (0x007DF3C2), 5:5:6 (0x007DF500), 6:5:5 (0x007DF640).
void FUN_007DF280(unsigned char * m, unsigned short * c, int mw, int cw, int w, int h, int *) { Spot_Brighten<5, 11, 6, 5, 5, 0>(m, c, mw, cw, w, h); }
void FUN_007DF3C2(unsigned char * m, unsigned short * c, int mw, int cw, int w, int h, int *) { Spot_Brighten<5, 10, 5, 5, 5, 0>(m, c, mw, cw, w, h); }
void FUN_007DF500(unsigned char * m, unsigned short * c, int mw, int cw, int w, int h, int *) { Spot_Brighten<5, 11, 5, 6, 6, 0>(m, c, mw, cw, w, h); }
void FUN_007DF640(unsigned char * m, unsigned short * c, int mw, int cw, int w, int h, int *) { Spot_Brighten<6, 10, 5, 5, 5, 0>(m, c, mw, cw, w, h); }

}
