// convert.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/convert.json carries the addresses and the rows that
// stay in their own files.


#define CONVERT_NAMED_BLITTERS
// convert.cpp unit (0x00483590-0x004861BF; TibSun convert.cpp): the empty
// rectangle of shared/shape_file.hpp (0x00851140) is this unit's first
// header-static initializer (0x00483590); ConvertClass::Drawers follows.
#define RGB_DECLARE_TO_HSV
#define CONVERT_DECLARE_DRAWERS
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/hsv_class.hpp"
#include "shared/shape_file.hpp"
#include "shared/vector_class.hpp"
#include "shared/convert.hpp"
#include "shared/hicolor.hpp"
#include "shared/surface.hpp"


// ConvertClass::Drawers (0x00851150): its initializer (0x004835B0) and guarded
// atexit destructor (0x004835F0) are emitted by this definition.
DynamicVectorClass<ConvertClass *> ConvertClass::Drawers;


// atexit destructor of ConvertClass::Drawers (0x004835F0): emitted by its definition.


// Builds indexed or hicolor translation tables and registers the converter.

ConvertClass::ConvertClass(PaletteClass const &artpalette, PaletteClass const &screenpalette,
                          Surface *surface, int levels, bool quick)
    : BytesPerPixel(surface->Bytes_Per_Pixel()), 
      blit_plain_xlat_008(0), blit_trans_xlat_00c(0), blit_trans_darken_010(0), blit_trans_zremap_xlat_014(0),
      blit_trans_lucent75_018(0), blit_trans_lucent50_01c(0), blit_trans_lucent25_020(0), blit_plain_xlat_zread_024(0),
      blit_trans_xlat_zread_028(0), blit_trans_darken_zread_02c(0), blit_trans_zremap_xlat_zread_030(0), blit_trans_lucent75_zread_034(0),
      blit_trans_lucent50_zread_038(0), blit_trans_lucent25_zread_03c(0), blit_trans_lucent75_zread_warp_040(0), blit_trans_lucent50_zread_warp_044(0),
      blit_trans_lucent25_zread_warp_048(0), blit_plain_xlat_zread_write_04c(0), blit_trans_xlat_zread_write_050(0), blit_trans_darken_zread_write_054(0),
      blit_trans_zremap_xlat_zread_write_058(0), blit_trans_lucent75_zread_write_05c(0), blit_trans_lucent50_zread_write_060(0), blit_trans_lucent25_zread_write_064(0),
      blit_plain_xlat_alpha_068(0), blit_trans_xlat_alpha_06c(0), blit_trans_zremap_xlat_alpha_070(0), blit_trans_lucent75_alpha_074(0),
      blit_trans_lucent50_alpha_078(0), blit_trans_lucent25_alpha_07c(0), blit_trans_xlat_write_alpha_080(0), blit_trans_xlat_mult_write_alpha_084(0),
      blit_translucent_write_alpha_088(0), blit_translucent50_nonzero_alpha_08c(0), blit_translucent50_zero_alpha_090(0), blit_plain_xlat_alpha_094(0),
      blit_trans_xlat_alpha_zread_098(0), blit_trans_zremap_xlat_alpha_zread_09c(0), blit_trans_lucent75_alpha_zread_0a0(0), blit_trans_lucent50_alpha_zread_0a4(0),
      blit_trans_lucent25_alpha_zread_0a8(0), blit_trans_lucent75_alpha_zread_warp_0ac(0), blit_trans_lucent50_alpha_zread_warp_0b0(0), blit_trans_lucent25_alpha_zread_warp_0b4(0),
      blit_plain_xlat_alpha_0b8(0), blit_trans_xlat_alpha_zread_write_0bc(0), blit_trans_zremap_xlat_alpha_zread_write_0c0(0), blit_trans_lucent75_alpha_zread_write_0c4(0),
      blit_trans_lucent50_alpha_zread_write_0c8(0), blit_trans_lucent25_alpha_zread_write_0cc(0), rleblit_trans_xlat_0d0(0), rleblit_trans_darken_0d4(0),
      rleblit_trans_zremap_xlat_0d8(0), rleblit_trans_lucent75_0dc(0), rleblit_trans_lucent50_0e0(0), rleblit_trans_lucent25_0e4(0),
      rleblit_trans_xlat_zread_0e8(0), rleblit_trans_zremap_xlat_zread_0ec(0), rleblit_trans_darken_zread_0f0(0), rleblit_trans_lucent75_zread_0f4(0),
      rleblit_trans_lucent50_zread_0f8(0), rleblit_trans_lucent25_zread_0fc(0), rleblit_trans_lucent75_zread_warp_100(0), rleblit_trans_lucent50_zread_warp_104(0),
      rleblit_trans_lucent25_zread_warp_108(0), rleblit_trans_xlat_zread_write_10c(0), rleblit_trans_zremap_xlat_zread_write_110(0), rleblit_trans_darken_zread_write_114(0),
      rleblit_trans_lucent75_zread_write_118(0), rleblit_trans_lucent50_zread_write_11c(0), rleblit_trans_lucent25_zread_write_120(0), rleblit_trans_xlat_alpha_124(0),
      rleblit_trans_zremap_xlat_alpha_128(0), rleblit_trans_lucent75_alpha_12c(0), rleblit_trans_lucent50_alpha_130(0), rleblit_trans_lucent25_alpha_134(0),
      rleblit_trans_xlat_alpha_zread_138(0), rleblit_trans_zremap_xlat_alpha_zread_13c(0), rleblit_trans_lucent75_alpha_zread_140(0), rleblit_trans_lucent50_alpha_zread_144(0),
      rleblit_trans_lucent25_alpha_zread_148(0), rleblit_trans_lucent75_alpha_zread_warp_14c(0), rleblit_trans_lucent50_alpha_zread_warp_150(0), rleblit_trans_lucent25_alpha_zread_warp_154(0),
      rleblit_trans_xlat_alpha_zread_write_158(0), rleblit_trans_zremap_xlat_alpha_zread_write_15c(0), rleblit_trans_lucent75_alpha_zread_write_160(0), rleblit_trans_lucent50_alpha_zread_write_164(0),
      rleblit_trans_lucent25_alpha_zread_write_168(0),
      unknown_shadow_count(levels), AlphaPalette(0), Table(0), Remap(0), ActiveRemap(0)
{
    if (unknown_shadow_count < 1) unknown_shadow_count = 1;
    if (BytesPerPixel == 1) {
        Remap = new unsigned char[256];
        const_cast<unsigned char *>(Remap)[0] = 0;
        for (int index = 1; index < 256; ++index) {
            HSVClass hsv = artpalette[index];
            hsv.Value /= 2;
            RGBClass color = hsv.To_RGB();
            const_cast<unsigned char *>(Remap)[index] = (unsigned char)artpalette.Closest_Color(color);
        }
        AlphaPalette = new unsigned char[256 * unknown_shadow_count];
        unsigned char *translation = (unsigned char *)AlphaPalette;
        Table = translation + 256 * ((unknown_shadow_count - 1) >> 1);
        if (unknown_shadow_count == 1) {
            *translation++ = 0;
            for (int index = 1; index < 256; ++index) {
                *translation++ = (unsigned char)screenpalette.Closest_Color(artpalette[index]);
            }
        } else {
            for (int level = 0; level <= unknown_shadow_count - 1; ++level) {
                *translation++ = 0;
                for (int index = 1; index < 256; ++index) {
                    HSVClass hsv = artpalette[index];
                    hsv.Value = (unsigned char)((level * hsv.Value * 2) / (unknown_shadow_count - 1));
                    RGBClass color = hsv.To_RGB();
                    *translation++ = (unsigned char)screenpalette.Closest_Color(color);
                }
            }
        }
    } else {
        AlphaPalette = new unsigned short[256 * unknown_shadow_count];
        unsigned short *translation = (unsigned short *)AlphaPalette;
        Table = translation + 256 * ((unknown_shadow_count - 1) >> 1);
        if (!quick) Build_Brightness_Tables(translation, unknown_shadow_count, artpalette);
        HalfMask = unknown_half_mask_852fd0;
        QuarterMask = unknown_quarter_mask_852fd2;
    }
    if (!quick) Initialize_Blitters_00483B00();
    Drawers.Add(this);
}


// Original initializer body: 0x00483B00..0x00485301; independently traced full CFG.
#include "shared/basic_depth_blitters.hpp"
#include "shared/basic_depth_warp_blitters.hpp"
#include "shared/basic_depth_write_blitters.hpp"
#include "shared/basic_lucent_blitters.hpp"
#include "shared/basic_rle_depth_blitters.hpp"
#include "shared/basic_rle_lucent_blitters.hpp"
#include "shared/basic_xlat_blitters.hpp"
#include "shared/byte_rle_blitters.hpp"
#include "shared/conditional_alpha_blitters.hpp"
#include "shared/destination_blitters.hpp"
#include "shared/translucent_write_alpha_blitter.hpp"
#include "shared/plain_xlat_alpha_blitter.hpp"
#include "shared/rle_active_alpha_depth.hpp"
#include "shared/rle_alpha_blend_depth.hpp"
#include "shared/rle_alpha_depth.hpp"
#include "shared/rle_blitter.hpp"
#include "shared/rle_byte_remaining.hpp"
#include "shared/rle_darken_depth.hpp"
#include "shared/rle_lucent_depth.hpp"
#include "shared/rle_remaining_writers.hpp"
#include "shared/rle_remap_depth.hpp"
#include "shared/trans_lucent_alpha_blitter.hpp"
#include "shared/trans_lucent_alpha_depth_blitter.hpp"
#include "shared/trans_lucent_alpha_warp_blitter.hpp"
#include "shared/trans_remap_xlat_alpha_blitter.hpp"
#include "shared/trans_remap_xlat_alpha_depth_blitter.hpp"
#include "shared/trans_xlat_alpha_blitter.hpp"
#include "shared/trans_xlat_alpha_depth_blitter.hpp"
#include "shared/trans_xlat_alpha_read_blitter.hpp"
#include "shared/xlat_alpha_blitter.hpp"
void ConvertClass::Initialize_Blitters_00483B00()
{
    if (BytesPerPixel == 1) {
        Blitters[0] = new BlitPlainXlat<unsigned char>((unsigned char const *)Table);
        Blitters[1] = new BlitTransXlat<unsigned char>((unsigned char const *)Table);
        Blitters[3] = new BlitTransZRemapXlat<unsigned char>((unsigned char const *const *)&ActiveRemap, (unsigned char const *)Table);
        Blitters[2] = new BlitTransRemapDest<unsigned char>((unsigned char const *)Remap);
        Blitters[4] = new BlitTransRemapXlat<unsigned char>((unsigned char const *)Remap, (unsigned char const *)Table);
        Blitters[5] = new BlitTransRemapXlat<unsigned char>((unsigned char const *)Remap, (unsigned char const *)Table);
        Blitters[6] = new BlitTransRemapXlat<unsigned char>((unsigned char const *)Remap, (unsigned char const *)Table);
        RLEBlitters[0] = new RLEBlitTransXlat<unsigned char>((unsigned char const *)Table);
        RLEBlitters[2] = new RLEBlitTransZRemapXlat<unsigned char>((unsigned char const *const *)&ActiveRemap, (unsigned char const *)Table);
        RLEBlitters[1] = new RLEBlitTransRemapDest<unsigned char>((unsigned char const *)Remap);
        RLEBlitters[3] = new RLEBlitTransRemapXlat<unsigned char>((unsigned char const *)Remap, (unsigned char const *)Table);
        RLEBlitters[4] = new RLEBlitTransRemapXlat<unsigned char>((unsigned char const *)Remap, (unsigned char const *)Table);
        RLEBlitters[5] = new RLEBlitTransRemapXlat<unsigned char>((unsigned char const *)Remap, (unsigned char const *)Table);
        RLEBlitters[6] = new RLEBlitTransXlatZRead<unsigned char>((unsigned char const *)Table);
        RLEBlitters[7] = new RLEBlitTransZRemapXlatZRead<unsigned char>((unsigned char const *const *)&ActiveRemap, (unsigned char const *)Table);
        RLEBlitters[8] = new RLEBlitTransRemapDestZRead<unsigned char>((unsigned char const *)Remap);
        RLEBlitters[9] = new RLEBlitTransRemapXlatZRead<unsigned char>((unsigned char const *)Remap, (unsigned char const *)Table);
        RLEBlitters[10] = new RLEBlitTransRemapXlatZRead<unsigned char>((unsigned char const *)Remap, (unsigned char const *)Table);
        RLEBlitters[11] = new RLEBlitTransRemapXlatZRead<unsigned char>((unsigned char const *)Remap, (unsigned char const *)Table);
        RLEBlitters[15] = new RLEBlitTransXlatZReadWrite<unsigned char>((unsigned char const *)Table);
        RLEBlitters[16] = new RLEBlitTransZRemapXlatZReadWrite<unsigned char>((unsigned char const *const *)&ActiveRemap, (unsigned char const *)Table);
        RLEBlitters[17] = new RLEBlitTransRemapDestZReadWrite<unsigned char>((unsigned char const *)Remap);
        RLEBlitters[18] = new RLEBlitTransRemapXlatZReadWrite<unsigned char>((unsigned char const *)Remap, (unsigned char const *)Table);
        RLEBlitters[19] = new RLEBlitTransRemapXlatZReadWrite<unsigned char>((unsigned char const *)Remap, (unsigned char const *)Table);
        RLEBlitters[20] = new RLEBlitTransRemapXlatZReadWrite<unsigned char>((unsigned char const *)Remap, (unsigned char const *)Table);
    } else {
        Blitters[0] = new BlitPlainXlat<unsigned short>((unsigned short const *)Table);
        Blitters[1] = new BlitTransXlat<unsigned short>((unsigned short const *)Table);
        Blitters[3] = new BlitTransZRemapXlat<unsigned short>((unsigned char const *const *)&ActiveRemap, (unsigned short const *)Table);
        Blitters[2] = new BlitTransDarken<unsigned short>(HalfMask);
        Blitters[4] = new BlitTransLucent75<unsigned short>((unsigned short const *)Table, QuarterMask);
        Blitters[5] = new BlitTransLucent50<unsigned short>((unsigned short const *)Table, HalfMask);
        Blitters[6] = new BlitTransLucent25<unsigned short>((unsigned short const *)Table, QuarterMask);
        Blitters[7] = new BlitPlainXlatZRead<unsigned short>((unsigned short const *)Table);
        Blitters[8] = new BlitTransXlatZRead<unsigned short>((unsigned short const *)Table);
        Blitters[10] = new BlitTransZRemapXlatZRead<unsigned short>((unsigned char const *const *)&ActiveRemap, (unsigned short const *)Table);
        Blitters[9] = new BlitTransDarkenZRead<unsigned short>(HalfMask);
        Blitters[11] = new BlitTransLucent75ZRead<unsigned short>((unsigned short const *)Table, QuarterMask);
        Blitters[12] = new BlitTransLucent50ZRead<unsigned short>((unsigned short const *)Table, HalfMask);
        Blitters[13] = new BlitTransLucent25ZRead<unsigned short>((unsigned short const *)Table, QuarterMask);
        Blitters[14] = new BlitTransLucent75ZReadWarp<unsigned short>((unsigned short const *)Table, QuarterMask);
        Blitters[15] = new BlitTransLucent50ZReadWarp<unsigned short>((unsigned short const *)Table, HalfMask);
        Blitters[16] = new BlitTransLucent25ZReadWarp<unsigned short>((unsigned short const *)Table, QuarterMask);
        Blitters[17] = new BlitPlainXlatZReadWrite<unsigned short>((unsigned short const *)Table);
        Blitters[18] = new BlitTransXlatZReadWrite<unsigned short>((unsigned short const *)Table);
        Blitters[20] = new BlitTransZRemapXlatZReadWrite<unsigned short>((unsigned char const *const *)&ActiveRemap, (unsigned short const *)Table);
        Blitters[19] = new BlitTransDarkenZReadWrite<unsigned short>(HalfMask);
        Blitters[21] = new BlitTransLucent75ZReadWrite<unsigned short>((unsigned short const *)Table, QuarterMask);
        Blitters[22] = new BlitTransLucent50ZReadWrite<unsigned short>((unsigned short const *)Table, HalfMask);
        Blitters[23] = new BlitTransLucent25ZReadWrite<unsigned short>((unsigned short const *)Table, QuarterMask);
        Blitters[24] = new BlitPlainXlatAlpha<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count);
        Blitters[25] = new BlitTransXlatAlpha<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count);
        Blitters[26] = new BlitTransZRemapXlatAlpha<unsigned short>((unsigned char const *const *)&ActiveRemap, (unsigned short const *)AlphaPalette, unknown_shadow_count);
        Blitters[27] = new BlitTransLucent75Alpha<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, QuarterMask);
        Blitters[28] = new BlitTransLucent50Alpha<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, HalfMask);
        Blitters[29] = new BlitTransLucent25Alpha<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, QuarterMask);
        Blitters[30] = new BlitTransXlatWriteAlpha<unsigned short>();
        Blitters[31] = new BlitTransXlatMultWriteAlpha<unsigned short>();
        Blitters[32] = new BlitTranslucentWriteAlpha<unsigned short>((unsigned short const *)AlphaPalette);
        Blitters[33] = new BlitTranslucent50NonzeroAlpha<unsigned short>((unsigned short const *)Table, HalfMask);
        Blitters[34] = new BlitTranslucent50ZeroAlpha<unsigned short>((unsigned short const *)Table, HalfMask);
        Blitters[35] = new BlitPlainXlatAlpha<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count);
        Blitters[36] = new BlitTransXlatAlphaZRead<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count);
        Blitters[37] = new BlitTransZRemapXlatAlphaZRead<unsigned short>((unsigned char const *const *)&ActiveRemap, (unsigned short const *)AlphaPalette, unknown_shadow_count);
        Blitters[38] = new BlitTransLucent75AlphaZRead<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, QuarterMask);
        Blitters[39] = new BlitTransLucent50AlphaZRead<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, HalfMask);
        Blitters[40] = new BlitTransLucent25AlphaZRead<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, QuarterMask);
        Blitters[41] = new BlitTransLucent75AlphaZReadWarp<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, QuarterMask);
        Blitters[42] = new BlitTransLucent50AlphaZReadWarp<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, HalfMask);
        Blitters[43] = new BlitTransLucent25AlphaZReadWarp<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, QuarterMask);
        Blitters[44] = new BlitPlainXlatAlpha<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count);
        Blitters[45] = new BlitTransXlatAlphaZReadWrite<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count);
        Blitters[46] = new BlitTransZRemapXlatAlphaZReadWrite<unsigned short>((unsigned char const *const *)&ActiveRemap, (unsigned short const *)AlphaPalette, unknown_shadow_count);
        Blitters[47] = new BlitTransLucent75AlphaZReadWrite<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, QuarterMask);
        Blitters[48] = new BlitTransLucent50AlphaZReadWrite<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, HalfMask);
        Blitters[49] = new BlitTransLucent25AlphaZReadWrite<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, QuarterMask);
        RLEBlitters[0] = new RLEBlitTransXlat<unsigned short>((unsigned short const *)Table);
        RLEBlitters[2] = new RLEBlitTransZRemapXlat<unsigned short>((unsigned char const *const *)&ActiveRemap, (unsigned short const *)Table);
        RLEBlitters[1] = new RLEBlitTransDarken<unsigned short>(HalfMask);
        RLEBlitters[3] = new RLEBlitTransLucent75<unsigned short>((unsigned short const *)Table, QuarterMask);
        RLEBlitters[4] = new RLEBlitTransLucent50<unsigned short>((unsigned short const *)Table, HalfMask);
        RLEBlitters[5] = new RLEBlitTransLucent25<unsigned short>((unsigned short const *)Table, QuarterMask);
        RLEBlitters[6] = new RLEBlitTransXlatZRead<unsigned short>((unsigned short const *)Table);
        RLEBlitters[7] = new RLEBlitTransZRemapXlatZRead<unsigned short>((unsigned char const *const *)&ActiveRemap, (unsigned short const *)Table);
        RLEBlitters[8] = new RLEBlitTransDarkenZRead<unsigned short>(HalfMask);
        RLEBlitters[9] = new RLEBlitTransLucent75ZRead<unsigned short>((unsigned short const *)Table, QuarterMask);
        RLEBlitters[10] = new RLEBlitTransLucent50ZRead<unsigned short>((unsigned short const *)Table, HalfMask);
        RLEBlitters[11] = new RLEBlitTransLucent25ZRead<unsigned short>((unsigned short const *)Table, QuarterMask);
        RLEBlitters[12] = new RLEBlitTransLucent75ZReadWarp<unsigned short>((unsigned short const *)Table, QuarterMask);
        RLEBlitters[13] = new RLEBlitTransLucent50ZReadWarp<unsigned short>((unsigned short const *)Table, HalfMask);
        RLEBlitters[14] = new RLEBlitTransLucent25ZReadWarp<unsigned short>((unsigned short const *)Table, QuarterMask);
        RLEBlitters[15] = new RLEBlitTransXlatZReadWrite<unsigned short>((unsigned short const *)Table);
        RLEBlitters[16] = new RLEBlitTransZRemapXlatZReadWrite<unsigned short>((unsigned char const *const *)&ActiveRemap, (unsigned short const *)Table);
        RLEBlitters[17] = new RLEBlitTransDarkenZReadWrite<unsigned short>(HalfMask);
        RLEBlitters[18] = new RLEBlitTransLucent75ZReadWrite<unsigned short>((unsigned short const *)Table, QuarterMask);
        RLEBlitters[19] = new RLEBlitTransLucent50ZReadWrite<unsigned short>((unsigned short const *)Table, HalfMask);
        RLEBlitters[20] = new RLEBlitTransLucent25ZReadWrite<unsigned short>((unsigned short const *)Table, QuarterMask);
        RLEBlitters[21] = new RLEBlitTransXlatAlpha<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count);
        RLEBlitters[22] = new RLEBlitTransZRemapXlatAlpha<unsigned short>((unsigned char const *const *)&ActiveRemap, (unsigned short const *)AlphaPalette, unknown_shadow_count);
        RLEBlitters[23] = new RLEBlitTransLucent75Alpha<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, QuarterMask);
        RLEBlitters[24] = new RLEBlitTransLucent50Alpha<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, HalfMask);
        RLEBlitters[25] = new RLEBlitTransLucent25Alpha<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, QuarterMask);
        RLEBlitters[26] = new RLEBlitTransXlatAlphaZRead<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count);
        RLEBlitters[27] = new RLEBlitTransZRemapXlatAlphaZRead<unsigned short>((unsigned char const *const *)&ActiveRemap, (unsigned short const *)AlphaPalette, unknown_shadow_count);
        RLEBlitters[28] = new RLEBlitTransLucent75AlphaZRead<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, QuarterMask);
        RLEBlitters[29] = new RLEBlitTransLucent50AlphaZRead<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, HalfMask);
        RLEBlitters[30] = new RLEBlitTransLucent25AlphaZRead<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, QuarterMask);
        RLEBlitters[31] = new RLEBlitTransLucent75AlphaZReadWarp<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, QuarterMask);
        RLEBlitters[32] = new RLEBlitTransLucent50AlphaZReadWarp<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, HalfMask);
        RLEBlitters[33] = new RLEBlitTransLucent25AlphaZReadWarp<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, QuarterMask);
        RLEBlitters[34] = new RLEBlitTransXlatAlphaZReadWrite<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count);
        RLEBlitters[35] = new RLEBlitTransZRemapXlatAlphaZReadWrite<unsigned short>((unsigned char const *const *)&ActiveRemap, (unsigned short const *)AlphaPalette, unknown_shadow_count);
        RLEBlitters[36] = new RLEBlitTransLucent75AlphaZReadWrite<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, QuarterMask);
        RLEBlitters[37] = new RLEBlitTransLucent50AlphaZReadWrite<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, HalfMask);
        RLEBlitters[38] = new RLEBlitTransLucent25AlphaZReadWrite<unsigned short>((unsigned short const *)AlphaPalette, unknown_shadow_count, QuarterMask);
    }
}


// ConvertClass::~ConvertClass (0x00485310; TibSun convert.cpp): frees the
// blitters and tables and takes the converter off Drawers. The deleting
// destructor (vtable 0x0079DF70 slot 0, 0x00486120) expands it inline.
ConvertClass::~ConvertClass()
{
    Delete_Blitters_004853A0();

    delete [] (unsigned char *)AlphaPalette;
    AlphaPalette = 0;

    delete [] (unsigned char *)Remap;
    Remap = 0;

    Drawers.Delete(this);
}


// void ConvertClass::Delete_Blitters_004853A0() (0x004853A0)
// Deletes every blitter the converter made (Initialize_Blitters_00483B00) and clears its slot;
// the destructor 0x00485310 calls it first. Blitters 33 and 34 are released before 30-32.
#include "shared/blitter.hpp"

void ConvertClass::Delete_Blitters_004853A0()
{
    delete Blitters[0];
    Blitters[0] = 0;
    delete Blitters[1];
    Blitters[1] = 0;
    delete Blitters[2];
    Blitters[2] = 0;
    delete Blitters[3];
    Blitters[3] = 0;
    delete Blitters[4];
    Blitters[4] = 0;
    delete Blitters[5];
    Blitters[5] = 0;
    delete Blitters[6];
    Blitters[6] = 0;
    delete Blitters[7];
    Blitters[7] = 0;
    delete Blitters[8];
    Blitters[8] = 0;
    delete Blitters[9];
    Blitters[9] = 0;
    delete Blitters[10];
    Blitters[10] = 0;
    delete Blitters[11];
    Blitters[11] = 0;
    delete Blitters[12];
    Blitters[12] = 0;
    delete Blitters[13];
    Blitters[13] = 0;
    delete Blitters[14];
    Blitters[14] = 0;
    delete Blitters[15];
    Blitters[15] = 0;
    delete Blitters[16];
    Blitters[16] = 0;
    delete Blitters[17];
    Blitters[17] = 0;
    delete Blitters[18];
    Blitters[18] = 0;
    delete Blitters[19];
    Blitters[19] = 0;
    delete Blitters[20];
    Blitters[20] = 0;
    delete Blitters[21];
    Blitters[21] = 0;
    delete Blitters[22];
    Blitters[22] = 0;
    delete Blitters[23];
    Blitters[23] = 0;
    delete Blitters[24];
    Blitters[24] = 0;
    delete Blitters[25];
    Blitters[25] = 0;
    delete Blitters[26];
    Blitters[26] = 0;
    delete Blitters[27];
    Blitters[27] = 0;
    delete Blitters[28];
    Blitters[28] = 0;
    delete Blitters[29];
    Blitters[29] = 0;
    delete Blitters[33];
    Blitters[33] = 0;
    delete Blitters[34];
    Blitters[34] = 0;
    delete Blitters[30];
    Blitters[30] = 0;
    delete Blitters[31];
    Blitters[31] = 0;
    delete Blitters[32];
    Blitters[32] = 0;
    delete Blitters[35];
    Blitters[35] = 0;
    delete Blitters[36];
    Blitters[36] = 0;
    delete Blitters[37];
    Blitters[37] = 0;
    delete Blitters[38];
    Blitters[38] = 0;
    delete Blitters[39];
    Blitters[39] = 0;
    delete Blitters[40];
    Blitters[40] = 0;
    delete Blitters[41];
    Blitters[41] = 0;
    delete Blitters[42];
    Blitters[42] = 0;
    delete Blitters[43];
    Blitters[43] = 0;
    delete Blitters[44];
    Blitters[44] = 0;
    delete Blitters[45];
    Blitters[45] = 0;
    delete Blitters[46];
    Blitters[46] = 0;
    delete Blitters[47];
    Blitters[47] = 0;
    delete Blitters[48];
    Blitters[48] = 0;
    delete Blitters[49];
    Blitters[49] = 0;
    delete RLEBlitters[0];
    RLEBlitters[0] = 0;
    delete RLEBlitters[1];
    RLEBlitters[1] = 0;
    delete RLEBlitters[2];
    RLEBlitters[2] = 0;
    delete RLEBlitters[3];
    RLEBlitters[3] = 0;
    delete RLEBlitters[4];
    RLEBlitters[4] = 0;
    delete RLEBlitters[5];
    RLEBlitters[5] = 0;
    delete RLEBlitters[6];
    RLEBlitters[6] = 0;
    delete RLEBlitters[7];
    RLEBlitters[7] = 0;
    delete RLEBlitters[8];
    RLEBlitters[8] = 0;
    delete RLEBlitters[9];
    RLEBlitters[9] = 0;
    delete RLEBlitters[10];
    RLEBlitters[10] = 0;
    delete RLEBlitters[11];
    RLEBlitters[11] = 0;
    delete RLEBlitters[12];
    RLEBlitters[12] = 0;
    delete RLEBlitters[13];
    RLEBlitters[13] = 0;
    delete RLEBlitters[14];
    RLEBlitters[14] = 0;
    delete RLEBlitters[15];
    RLEBlitters[15] = 0;
    delete RLEBlitters[16];
    RLEBlitters[16] = 0;
    delete RLEBlitters[17];
    RLEBlitters[17] = 0;
    delete RLEBlitters[18];
    RLEBlitters[18] = 0;
    delete RLEBlitters[19];
    RLEBlitters[19] = 0;
    delete RLEBlitters[20];
    RLEBlitters[20] = 0;
    delete RLEBlitters[21];
    RLEBlitters[21] = 0;
    delete RLEBlitters[22];
    RLEBlitters[22] = 0;
    delete RLEBlitters[23];
    RLEBlitters[23] = 0;
    delete RLEBlitters[24];
    RLEBlitters[24] = 0;
    delete RLEBlitters[25];
    RLEBlitters[25] = 0;
    delete RLEBlitters[26];
    RLEBlitters[26] = 0;
    delete RLEBlitters[27];
    RLEBlitters[27] = 0;
    delete RLEBlitters[28];
    RLEBlitters[28] = 0;
    delete RLEBlitters[29];
    RLEBlitters[29] = 0;
    delete RLEBlitters[30];
    RLEBlitters[30] = 0;
    delete RLEBlitters[31];
    RLEBlitters[31] = 0;
    delete RLEBlitters[32];
    RLEBlitters[32] = 0;
    delete RLEBlitters[33];
    RLEBlitters[33] = 0;
    delete RLEBlitters[34];
    RLEBlitters[34] = 0;
    delete RLEBlitters[35];
    RLEBlitters[35] = 0;
    delete RLEBlitters[36];
    RLEBlitters[36] = 0;
    delete RLEBlitters[37];
    RLEBlitters[37] = 0;
    delete RLEBlitters[38];
    RLEBlitters[38] = 0;
}


// ConvertClass::Select_Blitter_00485AA0 (0x00485AA0; TibSun Blitter_From_Flags):
// the blitter for a combination of shape drawing flags (remap 0x10, darken 1,
// translucency 2/4/6, plain 0x20, z write 0x4000, z read 0x1000|0x2000,
// alpha 0x800, warp 8); the blitters are made on first use.
Blitter *ConvertClass::Select_Blitter_00485AA0(unsigned flags)
{
    static unsigned z_read_flags = 0x3000;

    if (Blitters[0] == 0) {
        Initialize_Blitters_00483B00();
    }

    if (flags & 0x10) {
        if (flags & 0x4000) {
            if (flags & 0x800) {
                return Blitters[46];
            }
            return Blitters[20];
        }
        if (flags & z_read_flags) {
            if (flags & 0x800) {
                return Blitters[37];
            }
            return Blitters[10];
        }
        if (flags & 0x800) {
            return Blitters[26];
        }
        return Blitters[3];
    }

    switch (flags & 6) {
    case 2:
        if (flags & 0x4000) {
            if (flags & 0x800) {
                return Blitters[49];
            }
            return Blitters[23];
        }
        if (flags & z_read_flags) {
            if (flags & 8) {
                if (flags & 0x800) {
                    return Blitters[43];
                }
                return Blitters[16];
            }
            if (flags & 0x800) {
                return Blitters[40];
            }
            return Blitters[13];
        }
        if (flags & 0x800) {
            return Blitters[29];
        }
        return Blitters[6];

    case 4:
        if (flags & 0x4000) {
            if (flags & 0x800) {
                return Blitters[48];
            }
            return Blitters[22];
        }
        if (flags & z_read_flags) {
            if (flags & 8) {
                if (flags & 0x800) {
                    return Blitters[42];
                }
                return Blitters[15];
            }
            if (flags & 0x800) {
                return Blitters[39];
            }
            return Blitters[12];
        }
        if (flags & 0x800) {
            return Blitters[28];
        }
        if (flags & 0x10000) {
            return Blitters[34];
        }
        if (flags & 0x20000) {
            return Blitters[33];
        }
        return Blitters[5];

    case 6:
        if (flags & 0x4000) {
            if (flags & 0x800) {
                return Blitters[47];
            }
            return Blitters[21];
        }
        if (flags & z_read_flags) {
            if (flags & 8) {
                if (flags & 0x800) {
                    return Blitters[41];
                }
                return Blitters[14];
            }
            if (flags & 0x800) {
                return Blitters[38];
            }
            return Blitters[11];
        }
        if (flags & 0x800) {
            return Blitters[27];
        }
        return Blitters[4];
    }

    if (flags & 1) {
        if (flags & 0x4000) {
            return Blitters[19];
        }
        if (flags & z_read_flags) {
            return Blitters[9];
        }
        return Blitters[2];
    }

    if (flags & 0x20) {
        if (flags & 0x4000) {
            if (flags & 0x800) {
                return Blitters[45];
            }
            return Blitters[18];
        }
        if (flags & z_read_flags) {
            if (flags & 0x800) {
                return Blitters[36];
            }
            return Blitters[8];
        }
        if (flags & 0x800) {
            return Blitters[24];
        }
        return Blitters[0];
    }

    if (flags & 0x4000) {
        if (flags & 0x800) {
            return Blitters[45];
        }
        return Blitters[18];
    }
    if (flags & z_read_flags) {
        if (flags & 0x800) {
            return Blitters[36];
        }
        return Blitters[8];
    }
    if (flags & 0x8000) {
        return Blitters[30];
    }
    if (flags & 0x100) {
        return Blitters[31];
    }
    if (flags & 0x40) {
        return Blitters[32];
    }
    if (flags & 0x800) {
        return Blitters[25];
    }
    return Blitters[1];
}


// ConvertClass::RLEBlitter_From_Flags_00485d60 (0x00485D60; TibSun
// RLEBlitter_From_Flags): the RLE blitter for a combination of shape drawing
// flags (see Select_Blitter_00485AA0); the blitters are made on first use.
RLEBlitter const *ConvertClass::RLEBlitter_From_Flags_00485d60(unsigned flags) const
{
    static unsigned z_read_flags = 0x3000;

    if (Blitters[0] == 0) {
        ((ConvertClass *)this)->Initialize_Blitters_00483B00();
    }

    if (flags & 0x10) {
        if (flags & 0x4000) {
            if (flags & 0x800) {
                return RLEBlitters[35];
            }
            return RLEBlitters[16];
        }
        if (flags & z_read_flags) {
            if (flags & 0x800) {
                return RLEBlitters[27];
            }
            return RLEBlitters[7];
        }
        if (flags & 0x800) {
            return RLEBlitters[22];
        }
        return RLEBlitters[2];
    }

    switch (flags & 6) {
    case 2:
        if (flags & 0x4000) {
            if (flags & 0x800) {
                return RLEBlitters[38];
            }
            return RLEBlitters[20];
        }
        if (flags & z_read_flags) {
            if (flags & 8) {
                if (flags & 0x800) {
                    return RLEBlitters[33];
                }
                return RLEBlitters[14];
            }
            if (flags & 0x800) {
                return RLEBlitters[30];
            }
            return RLEBlitters[11];
        }
        if (flags & 0x800) {
            return RLEBlitters[25];
        }
        return RLEBlitters[5];

    case 4:
        if (flags & 0x4000) {
            if (flags & 0x800) {
                return RLEBlitters[37];
            }
            return RLEBlitters[19];
        }
        if (flags & z_read_flags) {
            if (flags & 8) {
                if (flags & 0x800) {
                    return RLEBlitters[32];
                }
                return RLEBlitters[13];
            }
            if (flags & 0x800) {
                return RLEBlitters[29];
            }
            return RLEBlitters[10];
        }
        if (flags & 0x800) {
            return RLEBlitters[24];
        }
        return RLEBlitters[4];

    case 6:
        if (flags & 0x4000) {
            if (flags & 0x800) {
                return RLEBlitters[36];
            }
            return RLEBlitters[18];
        }
        if (flags & z_read_flags) {
            if (flags & 8) {
                if (flags & 0x800) {
                    return RLEBlitters[31];
                }
                return RLEBlitters[12];
            }
            if (flags & 0x800) {
                return RLEBlitters[28];
            }
            return RLEBlitters[9];
        }
        if (flags & 0x800) {
            return RLEBlitters[23];
        }
        return RLEBlitters[3];
    }

    if (flags & 1) {
        if (flags & 0x4000) {
            return RLEBlitters[17];
        }
        if (flags & z_read_flags) {
            return RLEBlitters[8];
        }
        return RLEBlitters[1];
    }

    if (flags & 0x20) {
        if (flags & 0x4000) {
            if (flags & 0x800) {
                return RLEBlitters[34];
            }
            return RLEBlitters[15];
        }
        if (flags & z_read_flags) {
            if (flags & 0x800) {
                return RLEBlitters[26];
            }
            return RLEBlitters[6];
        }
        if (flags & 0x800) {
            return RLEBlitters[21];
        }
        return RLEBlitters[0];
    }

    if (flags & 0x4000) {
        if (flags & 0x800) {
            return RLEBlitters[34];
        }
        return RLEBlitters[15];
    }
    if (flags & z_read_flags) {
        if (flags & 0x800) {
            return RLEBlitters[26];
        }
        return RLEBlitters[6];
    }
    if (flags & 0x800) {
        return RLEBlitters[21];
    }
    return RLEBlitters[0];
}


// Unknown_00486010 (0x00486010; TibSun ConvertClass::Reinitialize_Hicolor_Tables):
// after a display format change every 16-bit converter's colour table is
// unpacked with the old format's shifts and repacked for the new format; the
// masks and blitters are rebuilt.
void __fastcall Unknown_00486010(int red_right, int red_left, int green_right, int green_left, int blue_right, int blue_left);

void __fastcall Unknown_00486010(int red_right, int red_left, int green_right, int green_left, int blue_right, int blue_left)
{
    for (int i = 0; i < ConvertClass::Drawers.Count(); i++) {
        ConvertClass *converter = ConvertClass::Drawers[i];
        if (converter->BytesPerPixel == 2) {
            unsigned short *table = (unsigned short *)converter->AlphaPalette;
            for (int index = 0; index < 256; index++) {
                unsigned short color = table[index];

                RGBClass rgb;
                rgb.Red = (unsigned char)((color >> red_left) << red_right);
                rgb.Green = (unsigned char)((color >> green_left) << green_right);
                rgb.Blue = (unsigned char)((color >> blue_left) << blue_right);

                // Repacked green first (not shared/hicolor.hpp's Build_Hicolor_Pixel order).
                table[index] = (unsigned short)(((rgb.Green >> unknown_shift_852fcc) << unknown_shift_852fc8)
                                              | ((rgb.Red >> unknown_shift_852fbc) << unknown_shift_852fb8)
                                              | ((rgb.Blue >> unknown_shift_852fc4) << unknown_shift_852fc0));
            }

            converter->HalfMask = unknown_half_mask_852fd0;
            converter->QuarterMask = unknown_quarter_mask_852fd2;

            converter->Delete_Blitters_004853A0();
            converter->Initialize_Blitters_00483B00();
        }
    }
}


// ConvertClass scalar deleting destructor (0x00486120, vtable 0x0079DF70 slot 0):
// emitted by the virtual destructor.


#include "shared/blit.hpp"
#include "shared/rgb_class.hpp"


// Blend expanded RGB channels using the wrapping alpha buffer.
inline RGBClass Expand_Hicolor_Pixel(unsigned short pixel)
{
    return RGBClass((unsigned char)((pixel >> unknown_shift_852fb8) << unknown_shift_852fbc),
                    (unsigned char)((pixel >> unknown_shift_852fc8) << unknown_shift_852fcc),
                    (unsigned char)((pixel >> unknown_shift_852fc0) << unknown_shift_852fc4));
}
template <>
void BlitTranslucentWriteAlpha<unsigned short>::BlitForward(void *target, void const *source, int length,
                                                          int, void *, void *buffer, int, int) const
{
    unsigned short *alpha = (unsigned short *)buffer;
    if (length > 0) {
        int count = length;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            if (color) {
                unsigned int weight = *alpha;
                unsigned int inverse = 255 - weight;
                if (weight == 255) weight = 256;
                RGBClass foreground = Expand_Hicolor_Pixel(Palette[color]);
                RGBClass background = Expand_Hicolor_Pixel(*(unsigned short *)target);
                int red = (int)(background.Red * inverse + foreground.Red * weight) >> 8;
                int green = (int)(background.Green * inverse + foreground.Green * weight) >> 8;
                int blue = (int)(background.Blue * inverse + foreground.Blue * weight) >> 8;
                if (blue > 255) blue = 255;
                int bounded_green = green > 255 ? 255 : green;
                *(unsigned short *)target = (unsigned short)Build_Hicolor_Pixel(red > 255 ? 255 : red, bounded_green, blue);
            }
            target = (unsigned short *)target + 1;
            ++alpha;
            alpha = AlphaBuffer->Wrap_Pointer(alpha);
        } while (--count);
    }
}


// 0048c7d0: BlitPlainXlat_unsigned_char scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048c7f0: BlitTransXlat_unsigned_char scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048c810: BlitTransZRemapXlat_unsigned_char scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048c830: BlitTransRemapDest_unsigned_char scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048c850: BlitTransRemapXlat_unsigned_char scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048c870: RLEBlitTransXlat_unsigned_char scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048c8b0: RLEBlitTransZRemapXlat_unsigned_char scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048c8d0: RLEBlitTransRemapDest_unsigned_char scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048c8f0: RLEBlitTransRemapXlat_unsigned_char scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048c910: RLEBlitTransXlatZRead_unsigned_char scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048c930: RLEBlitTransZRemapXlatZRead_unsigned_char scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048c950: RLEBlitTransRemapDestZRead_unsigned_char scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048c970: RLEBlitTransRemapXlatZRead_unsigned_char scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048c990: RLEBlitTransXlatZReadWrite_unsigned_char scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048c9b0: RLEBlitTransZRemapXlatZReadWrite_unsigned_char scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048c9d0: RLEBlitTransRemapDestZReadWrite_unsigned_char scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048c9f0: RLEBlitTransRemapXlatZReadWrite_unsigned_char scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048ca10: BlitPlainXlat_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048ca30: BlitTransZRemapXlat_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048ca50: BlitTransDarken_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048ca70: BlitTransLucent75_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048ca90: BlitTransLucent50_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048cab0: BlitTransLucent25_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048cad0: BlitPlainXlatZRead_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048caf0: BlitTransXlatZRead_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048cb10: BlitTransZRemapXlatZRead_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048cb30: BlitTransDarkenZRead_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048cb50: BlitTransLucent75ZRead_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048cb70: BlitTransLucent50ZRead_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048cb90: BlitTransLucent25ZRead_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048cbb0: BlitTransLucent75ZReadWarp_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048cbd0: BlitTransLucent50ZReadWarp_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048cbf0: BlitTransLucent25ZReadWarp_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048cc10: BlitPlainXlatZReadWrite_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048cc30: BlitTransXlatZReadWrite_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048cc50: BlitTransZRemapXlatZReadWrite_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048cc70: BlitTransDarkenZReadWrite_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048cc90: BlitTransLucent75ZReadWrite_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048ccb0: BlitTransLucent50ZReadWrite_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048ccd0: BlitTransLucent25ZReadWrite_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048ce90: BlitTransXlatMultWriteAlpha_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048ceb0: BlitTranslucentWriteAlpha_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048ced0: BlitTranslucent50NonzeroAlpha_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048cef0: BlitTranslucent50ZeroAlpha_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d270: RLEBlitTransZRemapXlat_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d290: RLEBlitTransDarken_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d2b0: RLEBlitTransLucent75_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d2d0: RLEBlitTransLucent50_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d2f0: RLEBlitTransLucent25_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d310: RLEBlitTransXlatZRead_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d330: RLEBlitTransZRemapXlatZRead_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d350: RLEBlitTransDarkenZRead_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d370: RLEBlitTransLucent75ZRead_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d390: RLEBlitTransLucent50ZRead_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d3b0: RLEBlitTransLucent25ZRead_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d3d0: RLEBlitTransLucent75ZReadWarp_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d3f0: RLEBlitTransLucent50ZReadWarp_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d410: RLEBlitTransLucent25ZReadWarp_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d430: RLEBlitTransXlatZReadWrite_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d450: RLEBlitTransZRemapXlatZReadWrite_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d470: RLEBlitTransDarkenZReadWrite_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d490: RLEBlitTransLucent75ZReadWrite_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d4b0: RLEBlitTransLucent50ZReadWrite_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// 0048d4d0: RLEBlitTransLucent25ZReadWrite_unsigned_short scalar deleting destructor.
// Emitted naturally by its new expression in ConvertClass_Initialize_Blitters_00483b00.cpp.


// Byte-output translation; class naming hypothesis documented in the shared header.
template <>
void BlitPlainXlat<unsigned char>::BlitForward(void *target, void const *source, int length,
                                      int, void *, void *, int, int) const
{
    if (length > 0) {
        int count = length;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            *(unsigned char *)target = Palette[color];
            target = (unsigned char *)target + 1;
        } while (--count);
    }
}


// Byte-output translation; class naming hypothesis documented in the shared header.
template <>
void BlitTransXlat<unsigned char>::BlitForward(void *target, void const *source, int length,
                                      int, void *, void *, int, int) const
{
    if (length > 0) {
        int count = length;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            if (color) {
                *(unsigned char *)target = Palette[color];
            }
            target = (unsigned char *)target + 1;
        } while (--count);
    }
}


// Byte-output translation; class naming hypothesis documented in the shared header.
template <>
void BlitTransZRemapXlat<unsigned char>::BlitForward(void *target, void const *source, int length,
                                      int, void *, void *, int, int) const
{
    unsigned char const *remap = *RemapTable;
    if (length > 0) {
        int count = length;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            if (color) {
                *(unsigned char *)target = Palette[remap[color]];
            }
            target = (unsigned char *)target + 1;
        } while (--count);
    }
}


// Translate or darken the destination where the source is nonzero.
template <>
void BlitTransRemapDest<unsigned char>::BlitForward(void *target, void const *source, int length,
                              int, void *, void *, int, int) const
{
    if (length > 0) {
        int count = length;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            if (color) *(unsigned char *)target = Palette[*(unsigned char *)target];
            target = (unsigned char *)target + 1;
        } while (--count);
    }
}


// Byte-output translation; class naming hypothesis documented in the shared header.
template <>
void BlitTransRemapXlat<unsigned char>::BlitForward(void *target, void const *source, int length,
                                      int, void *, void *, int, int) const
{
    if (length > 0) {
        int count = length;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            if (color) {
                *(unsigned char *)target = Palette[RemapTable[color]];
            }
            target = (unsigned char *)target + 1;
        } while (--count);
    }
}


// Skip the clipped left portion of an RLE row, then translate its visible nonzero pixels.
template <>
void RLEBlitTransXlat<unsigned char>::Blit(void *target, void const *source, int length, int skip,
                                          int, void *, void *, int, int, void *) const
{
    int pixels_to_skip = skip;
    unsigned char *dest = (unsigned char *)target;
    unsigned char const *input = (unsigned char const *)source;
    if (pixels_to_skip > 0) {
        do {
            if (*input++ == 0) {
                pixels_to_skip -= *input++;
            } else {
                pixels_to_skip--;
            }
        } while (pixels_to_skip > 0);
        pixels_to_skip = -pixels_to_skip;
        dest += pixels_to_skip;
        length -= pixels_to_skip;
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
        } else {
            *dest++ = Palette[color];
            length--;
        }
    }
}


// Skip the clipped left portion of an RLE row, then translate its visible nonzero pixels.
template <>
void RLEBlitTransZRemapXlat<unsigned char>::Blit(void *target, void const *source, int length, int skip,
                                          int, void *, void *, int, int, void *) const
{
    unsigned char const *remap = *RemapTable;
    int pixels_to_skip = skip;
    unsigned char *dest = (unsigned char *)target;
    unsigned char const *input = (unsigned char const *)source;
    if (pixels_to_skip > 0) {
        do {
            if (*input++ == 0) {
                pixels_to_skip -= *input++;
            } else {
                pixels_to_skip--;
            }
        } while (pixels_to_skip > 0);
        pixels_to_skip = -pixels_to_skip;
        dest += pixels_to_skip;
        length -= pixels_to_skip;
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
        } else {
            *dest++ = Palette[remap[color]];
            length--;
        }
    }
}


// Skip the clipped left portion of an RLE row, then translate its visible nonzero pixels.
template <>
void RLEBlitTransRemapDest<unsigned char>::Blit(void *target, void const *source, int length, int skip,
                                          int, void *, void *, int, int, void *) const
{
    int pixels_to_skip = skip;
    unsigned char *dest = (unsigned char *)target;
    unsigned char const *input = (unsigned char const *)source;
    if (pixels_to_skip > 0) {
        do {
            if (*input++ == 0) {
                pixels_to_skip -= *input++;
            } else {
                pixels_to_skip--;
            }
        } while (pixels_to_skip > 0);
        pixels_to_skip = -pixels_to_skip;
        dest += pixels_to_skip;
        length -= pixels_to_skip;
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
        } else {
            *dest = Palette[*dest];
            length--;
            ++dest;
        }
    }
}


// Skip the clipped left portion of an RLE row, then translate its visible nonzero pixels.
template <>
void RLEBlitTransRemapXlat<unsigned char>::Blit(void *target, void const *source, int length, int skip,
                                          int, void *, void *, int, int, void *) const
{
    int pixels_to_skip = skip;
    unsigned char *dest = (unsigned char *)target;
    unsigned char const *input = (unsigned char const *)source;
    if (pixels_to_skip > 0) {
        do {
            if (*input++ == 0) {
                pixels_to_skip -= *input++;
            } else {
                pixels_to_skip--;
            }
        } while (pixels_to_skip > 0);
        pixels_to_skip = -pixels_to_skip;
        dest += pixels_to_skip;
        length -= pixels_to_skip;
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
        } else {
            *dest++ = Palette[RemapTable[color]];
            length--;
        }
    }
}


// Decode visible RLE pixels after a signed adjustment/depth test; no depth write.
template <>
void RLEBlitTransXlatZRead<unsigned char>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int, void *adjustment) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    char const *delta = (char const *)adjustment;
    unsigned char *output=(unsigned char *)target;
    unsigned char const *input = (unsigned char const *)source;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned char *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned char *)output + color;
            depth_ptr += color;
            delta += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                *(unsigned char *)output = Palette[color];
                ++output;
                --length;
            } else {
                ++output; --length;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Snapshot the shared remap once, then translate visible RLE pixels after a depth test.
// Opaque reads consume one signed adjustment byte; zero runs advance all three cursors.
template <>
void RLEBlitTransZRemapXlatZRead<unsigned char>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int, void *adjustment) const
{
    unsigned char const *input = (unsigned char const *)source;
    char const *delta = (char const *)adjustment;
    unsigned char *output=(unsigned char *)target;
    unsigned char const *remap=*RemapTable;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned char *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned char *)output + color;
            delta += color;
            depth_ptr += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                *(unsigned char *)output = Palette[remap[color]];
                ++output;
                --length;
            } else {
                ++output; --length;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Decode visible RLE pixels after a signed adjustment/depth test; no depth write.
template <>
void RLEBlitTransRemapDestZRead<unsigned char>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int, void *adjustment) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    char const *delta = (char const *)adjustment;
    unsigned char *output=(unsigned char *)target;
    unsigned char const *input = (unsigned char const *)source;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned char *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned char *)output + color;
            delta += color;
            depth_ptr += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                unsigned char previous=*output;
                --length;
                *output++=Palette[previous];
            } else {
                ++output; --length;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Decode visible RLE pixels after a signed adjustment/depth test; no depth write.
template <>
void RLEBlitTransRemapXlatZRead<unsigned char>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int, void *adjustment) const
{
    unsigned char const *input = (unsigned char const *)source;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    char const *delta = (char const *)adjustment;
    unsigned char *output=(unsigned char *)target;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned char *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned char *)output + color;
            delta += color;
            depth_ptr += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                *(unsigned char *)output = Palette[RemapTable[color]];
                ++output;
                --length;
            } else {
                ++output; --length;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Decode an RLE row, depth-test each opaque pixel with its signed adjustment,
// and write depth after the pixel store. The depth cursor wraps in its ring.
template <>
void RLEBlitTransXlatZReadWrite<unsigned char>::Blit(void *target, void const *source, int length, int skip,
                                                    int depth, void *depth_buffer, void *buffer, int level, int, void *adjustments) const
{
    unsigned char const *input = (unsigned char const *)source;
    unsigned char *dest = (unsigned char *)target;
    char const *delta = (char const *)adjustments;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (skip > 0) {
        skip=Skip_RLE_Pixels(input,skip);
        skip = -skip;
        dest += skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        length -= skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
            delta += color;
            depth_ptr += color;
        } else {
            Write_Depth_Checked_Pixel(dest, depth_ptr, delta, depth, color, length);
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Snapshot the shared remap once, then translate visible RLE pixels after a depth test.
// Opaque writes consume two signed adjustment bytes; zero runs consume one per skipped pixel.
template <>
void RLEBlitTransZRemapXlatZReadWrite<unsigned char>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int, void *adjustment) const
{
    unsigned char const *input = (unsigned char const *)source;
    char const *delta = (char const *)adjustment;
    unsigned char *output=(unsigned char *)target;
    unsigned char const *remap=*RemapTable;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned char *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned char *)output + color;
            delta += color;
            depth_ptr += color;
        } else {
            if (depth - *delta++ < *depth_ptr) {
                *output++ = Palette[remap[color]];
                *depth_ptr++=(unsigned short)(depth-*delta++);
                --length;
            } else {
                ++delta;
                output=(unsigned char *)output+1;
                ++depth_ptr; --length;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Snapshot the shared remap once, then translate visible RLE pixels after a depth test.
// Opaque writes consume two signed adjustment bytes; zero runs consume one per skipped pixel.
template <>
void RLEBlitTransRemapDestZReadWrite<unsigned char>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int, void *adjustment) const
{
    unsigned char const *input = (unsigned char const *)source;
    char const *delta = (char const *)adjustment;
    unsigned char *output=(unsigned char *)target;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned char *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned char *)output + color;
            delta += color;
            depth_ptr += color;
        } else {
            if (depth - *delta++ < *depth_ptr) {
                *output = Palette[*output];
                *depth_ptr++=(unsigned short)(depth-*delta++);
                --length; ++output;
            } else {
                ++delta;
                output=(unsigned char *)output+1;
                ++depth_ptr; --length;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Translate depth-tested RLE pixels and store depth after each successful pixel store.
// Left clipping does not advance adjustments; transparent runs advance all three cursors.
template <>
void RLEBlitTransRemapXlatZReadWrite<unsigned char>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int, void *adjustment) const
{
    unsigned char const *input = (unsigned char const *)source;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned char *output=(unsigned char *)target;
    char const *delta = (char const *)adjustment;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned char *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned char *)output + color;
            delta += color;
            depth_ptr += color;
        } else {
            if (depth - *delta < *depth_ptr) {
                *output++ = Palette[RemapTable[color]];
                *depth_ptr++=(unsigned short)(depth-*delta);
                ++delta; --length;
            } else {
                ++delta;
                output=(unsigned char *)output+1;
                ++depth_ptr; --length;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Byte-output translation; class naming hypothesis documented in the shared header.
template <>
void BlitPlainXlat<unsigned short>::BlitForward(void *target, void const *source, int length,
                                      int, void *, void *, int, int) const
{
    if (length > 0) {
        int count = length;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            *(unsigned short *)target = Palette[color];
            target = (unsigned short *)target + 1;
        } while (--count);
    }
}


// Byte-output translation; class naming hypothesis documented in the shared header.
template <>
void BlitTransZRemapXlat<unsigned short>::BlitForward(void *target, void const *source, int length,
                                      int, void *, void *, int, int) const
{
    unsigned char const *remap = *RemapTable;
    if (length > 0) {
        int count = length;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            if (color) {
                *(unsigned short *)target = Palette[remap[color]];
            }
            target = (unsigned short *)target + 1;
        } while (--count);
    }
}


// Translate or darken the destination where the source is nonzero.
template <>
void BlitTransDarken<unsigned short>::BlitForward(void *target, void const *source, int length,
                              int, void *, void *, int, int) const
{
    if (length > 0) {
        int count = length;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            if (color) *(unsigned short *)target = (*(unsigned short *)target >> 1) & ChannelMask;
            target = (unsigned short *)target + 1;
        } while (--count);
    }
}


// Byte-output translation; class naming hypothesis documented in the shared header.
template <>
void BlitTransLucent75<unsigned short>::BlitForward(void *target, void const *source, int length,
                                      int, void *, void *, int, int) const
{
    if (length > 0) {
        int count = length;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            if (color) {
                *(unsigned short *)target = (unsigned short)((Palette[color] >> 2 & ChannelMask) + (*(unsigned short *)target >> 2 & ChannelMask) * 3);
            }
            target = (unsigned short *)target + 1;
        } while (--count);
    }
}


// Byte-output translation; class naming hypothesis documented in the shared header.
template <>
void BlitTransLucent25<unsigned short>::BlitForward(void *target, void const *source, int length,
                                      int, void *, void *, int, int) const
{
    if (length > 0) {
        int count = length;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            if (color) {
                *(unsigned short *)target = (unsigned short)((Palette[color] >> 2 & ChannelMask) * 3 + (*(unsigned short *)target >> 2 & ChannelMask));
            }
            target = (unsigned short *)target + 1;
        } while (--count);
    }
}


// Depth test before accessing the source byte; wrap the supplied depth scanline.
template <>
void BlitPlainXlatZRead<unsigned short>::BlitForward(void *target, void const *source, int length,
                                      int depth, void *depth_buffer, void *, int, int) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (length > 0) {
        int count = length;
        do {
            if (depth < *depth_ptr++) {
                unsigned char color = *(unsigned char const *)source;
                *(unsigned short *)target = Palette[color];
            }
            source = (unsigned char const *)source + 1;
            target = (unsigned short *)target + 1;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        } while (--count);
    }
}


// Depth test before accessing the source byte; wrap the supplied depth scanline.
template <>
void BlitTransXlatZRead<unsigned short>::BlitForward(void *target, void const *source, int length,
                                      int depth, void *depth_buffer, void *, int, int) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (length > 0) {
        int count = length;
        do {
            if (depth < *depth_ptr++) {
                unsigned char color = *(unsigned char const *)source;
                if (color) {
                    *(unsigned short *)target = Palette[color];
                }
            }
            source = (unsigned char const *)source + 1;
            target = (unsigned short *)target + 1;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        } while (--count);
    }
}


// Depth test before accessing the source byte; wrap the supplied depth scanline.
template <>
void BlitTransZRemapXlatZRead<unsigned short>::BlitForward(void *target, void const *source, int length,
                                      int depth, void *depth_buffer, void *, int, int) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned char const *remap = *RemapTable;
    if (length > 0) {
        int count = length;
        do {
            if (depth < *depth_ptr++) {
                unsigned char color = *(unsigned char const *)source;
                if (color) {
                    *(unsigned short *)target = Palette[remap[color]];
                }
            }
            source = (unsigned char const *)source + 1;
            target = (unsigned short *)target + 1;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        } while (--count);
    }
}


// Depth test before accessing the source byte; wrap the supplied depth scanline.
template <>
void BlitTransDarkenZRead<unsigned short>::BlitForward(void *target, void const *source, int length,
                                      int depth, void *depth_buffer, void *, int, int) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (length > 0) {
        int count = length;
        do {
            if (depth < *depth_ptr++) {
                unsigned char color = *(unsigned char const *)source;
                if (color) {
                    *(unsigned short *)target = (*(unsigned short *)target >> 1) & ChannelMask;
                }
            }
            source = (unsigned char const *)source + 1;
            target = (unsigned short *)target + 1;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        } while (--count);
    }
}


// Depth test before accessing the source byte; wrap the supplied depth scanline.
template <>
void BlitTransLucent75ZRead<unsigned short>::BlitForward(void *target, void const *source, int length,
                                      int depth, void *depth_buffer, void *, int, int) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (length > 0) {
        int count = length;
        do {
            if (depth < *depth_ptr++) {
                unsigned char color = *(unsigned char const *)source;
                if (color) {
                    *(unsigned short *)target = ((Palette[color] >> 2) & ChannelMask) + ((*(unsigned short *)target >> 2) & ChannelMask) * 3;
                }
            }
            source = (unsigned char const *)source + 1;
            target = (unsigned short *)target + 1;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        } while (--count);
    }
}


// Depth test before accessing the source byte; wrap the supplied depth scanline.
template <>
void BlitTransLucent50ZRead<unsigned short>::BlitForward(void *target, void const *source, int length,
                                      int depth, void *depth_buffer, void *, int, int) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (length > 0) {
        int count = length;
        do {
            if (depth < *depth_ptr++) {
                unsigned char color = *(unsigned char const *)source;
                if (color) {
                    *(unsigned short *)target = ((Palette[color] >> 1) & ChannelMask) + ((*(unsigned short *)target >> 1) & ChannelMask);
                }
            }
            source = (unsigned char const *)source + 1;
            target = (unsigned short *)target + 1;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        } while (--count);
    }
}


// Backward entry forwards through the virtual forward slot, with zero as the last argument.
template <>
void BlitTransLucent50ZRead<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                         int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// Depth test before accessing the source byte; wrap the supplied depth scanline.
template <>
void BlitTransLucent25ZRead<unsigned short>::BlitForward(void *target, void const *source, int length,
                                      int depth, void *depth_buffer, void *, int, int) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (length > 0) {
        int count = length;
        do {
            if (depth < *depth_ptr++) {
                unsigned char color = *(unsigned char const *)source;
                if (color) {
                    *(unsigned short *)target = ((Palette[color] >> 2) & ChannelMask) * 3 + ((*(unsigned short *)target >> 2) & ChannelMask);
                }
            }
            source = (unsigned char const *)source + 1;
            target = (unsigned short *)target + 1;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        } while (--count);
    }
}


// Depth test before accessing the source byte; wrap the supplied depth scanline.
template <>
void BlitTransLucent75ZReadWarp<unsigned short>::BlitForward(void *target, void const *source, int length,
                                      int depth, void *depth_buffer, void *, int, int warp_offset) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (length > 0) {
        int count = length;
        do {
            if (depth < *depth_ptr++) {
                unsigned char color = *(unsigned char const *)source;
                if (color) {
                    *(unsigned short *)target = ((Palette[color] >> 2) & ChannelMask) + ((((unsigned short *)target)[warp_offset] >> 2) & ChannelMask) * 3;
                }
            }
            source = (unsigned char const *)source + 1;
            target = (unsigned short *)target + 1;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        } while (--count);
    }
}


// Depth test before accessing the source byte; wrap the supplied depth scanline.
template <>
void BlitTransLucent50ZReadWarp<unsigned short>::BlitForward(void *target, void const *source, int length,
                                      int depth, void *depth_buffer, void *, int, int warp_offset) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (length > 0) {
        int count = length;
        do {
            if (depth < *depth_ptr++) {
                unsigned char color = *(unsigned char const *)source;
                if (color) {
                    *(unsigned short *)target = ((Palette[color] >> 1) & ChannelMask) + ((((unsigned short *)target)[warp_offset] >> 1) & ChannelMask);
                }
            }
            source = (unsigned char const *)source + 1;
            target = (unsigned short *)target + 1;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        } while (--count);
    }
}


// Depth test before accessing the source byte; wrap the supplied depth scanline.
template <>
void BlitTransLucent25ZReadWarp<unsigned short>::BlitForward(void *target, void const *source, int length,
                                      int depth, void *depth_buffer, void *, int, int warp_offset) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (length > 0) {
        int count = length;
        do {
            if (depth < *depth_ptr++) {
                unsigned char color = *(unsigned char const *)source;
                if (color) {
                    *(unsigned short *)target = ((Palette[color] >> 2) & ChannelMask) * 3 + ((((unsigned short *)target)[warp_offset] >> 2) & ChannelMask);
                }
            }
            source = (unsigned char const *)source + 1;
            target = (unsigned short *)target + 1;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        } while (--count);
    }
}


// Depth test before accessing the source byte; wrap the supplied depth scanline.
template <>
void BlitPlainXlatZReadWrite<unsigned short>::BlitForward(void *target, void const *source, int length,
                                      int depth, void *depth_buffer, void *, int, int) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (length > 0) {
        int count = length;
        do {
            if (depth < *depth_ptr) {
                unsigned char color = *(unsigned char const *)source;
                *(unsigned short *)target = Palette[color];
                *depth_ptr = (unsigned char)depth;
            }
            ++depth_ptr;
            source = (unsigned char const *)source + 1;
            target = (unsigned short *)target + 1;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        } while (--count);
    }
}


// Depth test before accessing the source byte; wrap the supplied depth scanline.
template <>
void BlitTransXlatZReadWrite<unsigned short>::BlitForward(void *target, void const *source, int length,
                                      int depth, void *depth_buffer, void *, int, int) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (length > 0) {
        int count = length;
        do {
            if (depth < *depth_ptr) {
                unsigned char color = *(unsigned char const *)source;
                if (color) {
                    *(unsigned short *)target = Palette[color];
                    *depth_ptr = (unsigned short)depth;
                }
            }
            ++depth_ptr;
            source = (unsigned char const *)source + 1;
            target = (unsigned short *)target + 1;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        } while (--count);
    }
}


// Depth test before accessing the source byte; wrap the supplied depth scanline.
template <>
void BlitTransZRemapXlatZReadWrite<unsigned short>::BlitForward(void *target, void const *source, int length,
                                      int depth, void *depth_buffer, void *, int, int) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned char const *remap = *RemapTable;
    if (length > 0) {
        int count = length;
        do {
            if (depth < *depth_ptr) {
                unsigned char color = *(unsigned char const *)source;
                if (color) {
                    *(unsigned short *)target = Palette[remap[color]];
                    *depth_ptr = (unsigned short)depth;
                }
            }
            ++depth_ptr;
            source = (unsigned char const *)source + 1;
            target = (unsigned short *)target + 1;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        } while (--count);
    }
}


// BlitPlainXlat<unsigned char>::BlitBackward (0x004862E0): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitPlainXlat<unsigned char>::BlitBackward(void *target, void const *source, int length,
                                                int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransXlat<unsigned char>::BlitBackward (0x00486350): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransXlat<unsigned char>::BlitBackward(void *target, void const *source, int length,
                                                int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransZRemapXlat<unsigned char>::BlitBackward (0x004863D0): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransZRemapXlat<unsigned char>::BlitBackward(void *target, void const *source, int length,
                                                      int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransRemapDest<unsigned char>::BlitBackward (0x00486440): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransRemapDest<unsigned char>::BlitBackward(void *target, void const *source, int length,
                                                     int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransRemapXlat<unsigned char>::BlitBackward (0x004864C0): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransRemapXlat<unsigned char>::BlitBackward(void *target, void const *source, int length,
                                                     int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitPlainXlat<unsigned short>::BlitBackward (0x00486ED0): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitPlainXlat<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                 int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransZRemapXlat<unsigned short>::BlitBackward (0x00486F50): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransZRemapXlat<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                       int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransDarken<unsigned short>::BlitBackward (0x00486FC0): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransDarken<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                   int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransLucent75<unsigned short>::BlitBackward (0x00487050): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransLucent75<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                     int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransLucent50<unsigned short>::BlitBackward (0x004870E0): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransLucent50<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                     int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransLucent25<unsigned short>::BlitBackward (0x00487170): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransLucent25<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                     int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitPlainXlatZRead<unsigned short>::BlitBackward (0x00487200): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitPlainXlatZRead<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                      int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransXlatZRead<unsigned short>::BlitBackward (0x004872A0): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransXlatZRead<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                      int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransZRemapXlatZRead<unsigned short>::BlitBackward (0x00487360): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransZRemapXlatZRead<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                            int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransDarkenZRead<unsigned short>::BlitBackward (0x004873F0): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransDarkenZRead<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                        int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransLucent75ZRead<unsigned short>::BlitBackward (0x004874B0): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransLucent75ZRead<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                          int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransLucent25ZRead<unsigned short>::BlitBackward (0x00487630): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransLucent25ZRead<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                          int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransLucent75ZReadWarp<unsigned short>::BlitBackward (0x00487700): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransLucent75ZReadWarp<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                              int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransLucent50ZReadWarp<unsigned short>::BlitBackward (0x004877D0): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransLucent50ZReadWarp<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                              int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransLucent25ZReadWarp<unsigned short>::BlitBackward (0x004878A0): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransLucent25ZReadWarp<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                              int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitPlainXlatZReadWrite<unsigned short>::BlitBackward (0x00487940): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitPlainXlatZReadWrite<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                           int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransXlatZReadWrite<unsigned short>::BlitBackward (0x004879F0): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransXlatZReadWrite<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                           int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransZRemapXlatZReadWrite<unsigned short>::BlitBackward (0x00487AB0): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransZRemapXlatZReadWrite<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                                 int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// 0x00487ae0: function not yet identified
//
// Body: 109 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void __thiscall BlitForward_00487ae0(int param_1,ushort *param_2,char *param_3,int param_4,int param_5,ushort *param_6)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// Adjusted from the Ghidra seed by permute_seeds.py (signedness, comparison order) to
// match the original; edit freely, this file is no longer regenerated.

namespace seed_00487ae0 {

extern "C" unsigned int DepthBuffer_00839c6c;

struct Owner_00487ae0 { void BlitForward_00487ae0(unsigned short *dest, char *source, int length, int z_min, unsigned short *z_buff, int a_buff, int alpha_level, int warp_offset); };

void Owner_00487ae0::BlitForward_00487ae0(unsigned short *dest, char *source, int length, int z_min, unsigned short *z_buff, int a_buff, int alpha_level, int warp_offset)
{
    int self = (int)this;
    unsigned short *z_ptr;
    int depth_buffer;

    z_ptr = (unsigned short *)(z_buff);
    if (0 < length) {
        z_buff = (unsigned short *)length;
        depth_buffer = (int)(DepthBuffer_00839c6c);
        do {
            if ((z_min < (int)(unsigned int)*z_ptr) && (*source != '\0')) {
                *dest = *dest >> 1 & *(unsigned short *)(self + 4);
                *z_ptr = (unsigned short)z_min;
                depth_buffer = (int)(DepthBuffer_00839c6c);
            }
            z_ptr = (unsigned short *)(z_ptr + 1);
            source = (char *)(source + 1);
            dest = (unsigned short *)(dest + 1);
            if (z_ptr >= *(unsigned short **)(depth_buffer + 0x1c)) {
                z_ptr = (unsigned short *)((int)z_ptr - *(int *)(depth_buffer + 0x20));
            }
            z_buff = (unsigned short *)((int)z_buff + -1);
        } while (z_buff != (unsigned short *)0x0);
    }
    return;
}

}  // namespace seed_00487ae0


// Depth test before accessing the source byte; wrap the supplied depth scanline.
template <>
void BlitTransLucent50ZReadWrite<unsigned short>::BlitForward(void *target, void const *source, int length,
                                      int depth, void *depth_buffer, void *, int, int) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (length > 0) {
        int count = length;
        do {
            if (depth < *depth_ptr) {
                unsigned char color = *(unsigned char const *)source;
                if (color) {
                    *(unsigned short *)target = ((Palette[color] >> 1) & ChannelMask) + ((*(unsigned short *)target >> 1) & ChannelMask);
                    *depth_ptr = (unsigned short)depth;
                }
            }
            ++depth_ptr;
            source = (unsigned char const *)source + 1;
            target = (unsigned short *)target + 1;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        } while (--count);
    }
}


// Translate every source index through the palette and the selected alpha lookup row.
template <>
void BlitPlainXlatAlpha<unsigned short>::BlitForward(void *target, void const *source, int length,
                                                   int, void *, void *buffer, int level, int) const
{
    unsigned short *alpha = (unsigned short *)buffer;
    int row = level > 0 ? level : 0;
    row = (row * 261) >> 11;
    if (row > 254) row = 254;
    unsigned short const *table = AlphaTable[row];
    if (length > 0) {
        unsigned char const *input = (unsigned char const *)source;
        unsigned short *dest = (unsigned short *)target;
        int count = length;
        do {
            unsigned char color = *input++;
            unsigned int value = *alpha++;
            *dest++ = Palette[table[value] | color];
            alpha = AlphaBuffer->Wrap_Pointer(alpha);
        } while (--count);
    }
}


// Translate nonzero source indices through the selected alpha row and palette.
template <>
void BlitTransXlatAlpha<unsigned short>::BlitForward(void *target, void const *source, int length,
                                                        int, void *, void *buffer, int level, int) const
{
    unsigned short *alpha = (unsigned short *)buffer;
    int row = level > 0 ? level : 0;
    row = (row * 261) >> 11;
    if (row > 254) row = 254;
    unsigned short const *table = AlphaTable[row];
    if (length > 0) {
        int count = length;
        unsigned short *dest = (unsigned short *)target;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            if (color) {
                unsigned int translated = table[*alpha];
                *dest = Palette[translated | color];
            }
            alpha++;
            dest++;
            alpha = AlphaBuffer->Wrap_Pointer(alpha);
        } while (--count);
    }
}


// Translate nonzero source indices through the selected alpha row and palette.
template <>
void BlitTransZRemapXlatAlpha<unsigned short>::BlitForward(void *target, void const *source, int length,
                                                        int, void *, void *buffer, int level, int) const
{
    unsigned short *alpha = (unsigned short *)buffer;
    unsigned char const *remap = *Remap;
    int row = level > 0 ? level : 0;
    row = (row * 261) >> 11;
    if (row > 254) row = 254;
    unsigned short const *table = AlphaTable[row];
    if (length > 0) {
        int count = length;
        unsigned short *dest = (unsigned short *)target;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            if (color) {
                unsigned int translated = table[*alpha];
                *dest = Palette[translated | remap[color]];
            }
            alpha++;
            dest++;
            alpha = AlphaBuffer->Wrap_Pointer(alpha);
        } while (--count);
    }
}


// Translate opaque indices through the alpha row and blend one quarter source, three quarters destination.
template <>
void BlitTransLucent75Alpha<unsigned short>::BlitForward(void *target, void const *source, int length,
                                                        int, void *, void *buffer, int level, int) const
{
    unsigned short *alpha = (unsigned short *)buffer;
    int row = level > 0 ? level : 0;
    row = (row * 261) >> 11;
    if (row > 254) row = 254;
    unsigned short const *table = AlphaTable[row];
    if (length > 0) {
        unsigned short *dest = (unsigned short *)target;
        int count = length;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            if (color) {
                unsigned int translated = table[*alpha];
                *dest = ((Palette[translated | color] >> 2) & ChannelMask) + ((*dest >> 2) & ChannelMask) * 3;
            }
            alpha++;
            dest++;
            alpha = AlphaBuffer->Wrap_Pointer(alpha);
        } while (--count);
    }
}


// Translate opaque indices through the alpha row and blend equal source and destination halves.
template <>
void BlitTransLucent50Alpha<unsigned short>::BlitForward(void *target, void const *source, int length,
                                                        int, void *, void *buffer, int level, int) const
{
    unsigned short *alpha = (unsigned short *)buffer;
    int row = level > 0 ? level : 0;
    row = (row * 261) >> 11;
    if (row > 254) row = 254;
    unsigned short const *table = AlphaTable[row];
    if (length > 0) {
        unsigned short *dest = (unsigned short *)target;
        int count = length;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            if (color) {
                unsigned int translated = table[*alpha];
                *dest = ((Palette[translated | color] >> 1) & ChannelMask) + ((*dest >> 1) & ChannelMask);
            }
            alpha++;
            dest++;
            alpha = AlphaBuffer->Wrap_Pointer(alpha);
        } while (--count);
    }
}


// Translate opaque indices through the alpha row and blend three quarters source, one quarter destination.
template <>
void BlitTransLucent25Alpha<unsigned short>::BlitForward(void *target, void const *source, int length,
                                                        int, void *, void *buffer, int level, int) const
{
    unsigned short *alpha = (unsigned short *)buffer;
    int row = level > 0 ? level : 0;
    row = (row * 261) >> 11;
    if (row > 254) row = 254;
    unsigned short const *table = AlphaTable[row];
    if (length > 0) {
        unsigned short *dest = (unsigned short *)target;
        int count = length;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            if (color) {
                unsigned int translated = table[*alpha];
                *dest = ((Palette[translated | color] >> 2) & ChannelMask) * 3 + ((*dest >> 2) & ChannelMask);
            }
            alpha++;
            dest++;
            alpha = AlphaBuffer->Wrap_Pointer(alpha);
        } while (--count);
    }
}


// Store clamped source-index-plus-level values for nonzero pixels, wrapping the buffer ring.
template <>
void BlitTransXlatWriteAlpha<unsigned short>::BlitForward(void *target, void const *source, int length,
                                                         int level, void *, void *buffer, int, int) const
{
    unsigned short *dest = (unsigned short *)buffer;
    if (length > 0) {
        int count = length;
        int base = level;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            if (color) {
                *dest = (unsigned short)(color + base >= 255 ? 255 : color + base);
            }
            dest++;
            dest = AlphaBuffer->Wrap_Pointer(dest);
        } while (--count);
    }
}


// Store clamped source-index-plus-level values for nonzero pixels, wrapping the buffer ring.
template <>
void BlitTransXlatMultWriteAlpha<unsigned short>::BlitForward(void *target, void const *source, int length,
                                                         int level, void *, void *buffer, int scale, int) const
{
    unsigned short *dest = (unsigned short *)buffer;
    if (length > 0) {
        int count = length;
        int base = level;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            if (color) {
                *dest = (unsigned short)(color * scale + base >= 255 ? 255 : color * scale + base);
            }
            dest++;
            dest = AlphaBuffer->Wrap_Pointer(dest);
        } while (--count);
    }
}


// Half-blend a nonzero source pixel only when its alpha value satisfies the mask condition.
template <>
void BlitTranslucent50NonzeroAlpha<unsigned short>::BlitForward(void *target, void const *source, int length,
                                       int, void *, void *buffer, int, int) const
{
    unsigned short *alpha = (unsigned short *)buffer;
    if (length > 0) {
        int count = length;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            if (color && *alpha != 0) {
                *(unsigned short *)target = ((Palette[color] >> 1) & ChannelMask)
                                           + ((*(unsigned short *)target >> 1) & ChannelMask);
            }
            ++alpha;
            target = (unsigned short *)target + 1;
            alpha = AlphaBuffer->Wrap_Pointer(alpha);
        } while (--count);
    }
}


// Half-blend a nonzero source pixel only when its alpha value satisfies the mask condition.
template <>
void BlitTranslucent50ZeroAlpha<unsigned short>::BlitForward(void *target, void const *source, int length,
                                       int, void *, void *buffer, int, int) const
{
    unsigned short *alpha = (unsigned short *)buffer;
    if (length > 0) {
        int count = length;
        do {
            unsigned char color = *(unsigned char const *)source;
            source = (unsigned char const *)source + 1;
            if (color && *alpha == 0) {
                *(unsigned short *)target = ((Palette[color] >> 1) & ChannelMask)
                                           + ((*(unsigned short *)target >> 1) & ChannelMask);
            }
            ++alpha;
            target = (unsigned short *)target + 1;
            alpha = AlphaBuffer->Wrap_Pointer(alpha);
        } while (--count);
    }
}


// Depth-test each source pixel before reading its index and translating through the alpha row.
template <>
void BlitTransXlatAlphaZRead<unsigned short>::BlitForward(void *target, void const *source, int length,
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
                    *(unsigned short *)target = Palette[translated | color];
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


// Depth-test each source pixel before reading its index and translating through the alpha row.
template <>
void BlitTransZRemapXlatAlphaZRead<unsigned short>::BlitForward(void *target, void const *source, int length,
                                                        int depth, void *depth_buffer, void *buffer, int level, int) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned short *alpha = (unsigned short *)buffer;
    unsigned char const *remap = *Remap;
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
                    *(unsigned short *)target = Palette[translated | remap[color]];
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


// Depth-test each source pixel before reading its index and translating through the alpha row.
template <>
void BlitTransLucent75AlphaZRead<unsigned short>::BlitForward(void *target, void const *source, int length,
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
                    *(unsigned short *)target = ((Palette[translated | color] >> 2) & ChannelMask) + ((*(unsigned short *)target >> 2) & ChannelMask) * 3;
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


// Depth-test each source pixel before reading its index and translating through the alpha row.
template <>
void BlitTransLucent50AlphaZRead<unsigned short>::BlitForward(void *target, void const *source, int length,
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
                    *(unsigned short *)target = ((Palette[translated | color] >> 1) & ChannelMask) + ((*(unsigned short *)target >> 1) & ChannelMask);
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


// Depth-test each source pixel before reading its index and translating through the alpha row.
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


// Depth-test each source pixel before reading its index and translating through the alpha row.
template <>
void BlitTransLucent75AlphaZReadWarp<unsigned short>::BlitForward(void *target, void const *source, int length,
                                                        int depth, void *depth_buffer, void *buffer, int level, int warp_offset) const
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
                    *(unsigned short *)target = ((Palette[translated | color] >> 2) & ChannelMask) + ((((unsigned short *)target)[warp_offset] >> 2) & ChannelMask) * 3;
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


// Depth-test each source pixel before reading its index and translating through the alpha row.
template <>
void BlitTransLucent50AlphaZReadWarp<unsigned short>::BlitForward(void *target, void const *source, int length,
                                                        int depth, void *depth_buffer, void *buffer, int level, int warp_offset) const
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
                    *(unsigned short *)target = ((Palette[translated | color] >> 1) & ChannelMask) + ((((unsigned short *)target)[warp_offset] >> 1) & ChannelMask);
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


// Depth-test each source pixel before reading its index and translating through the alpha row.
template <>
void BlitTransLucent25AlphaZReadWarp<unsigned short>::BlitForward(void *target, void const *source, int length,
                                                        int depth, void *depth_buffer, void *buffer, int level, int warp_offset) const
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
                    *(unsigned short *)target = ((Palette[translated | color] >> 2) & ChannelMask) * 3 + ((((unsigned short *)target)[warp_offset] >> 2) & ChannelMask);
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


// Depth-test each source pixel before reading its index and translating through the alpha row.
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


// Depth-test each source pixel before reading its index and translating through the alpha row.
template <>
void BlitTransZRemapXlatAlphaZReadWrite<unsigned short>::BlitForward(void *target, void const *source, int length,
                                                        int depth, void *depth_buffer, void *buffer, int level, int) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned short *alpha = (unsigned short *)buffer;
    int row = level > 0 ? level : 0;
    row = (row * 261) >> 11;
    if (row > 254) row = 254;
    unsigned short const *table = AlphaTable[row];
    unsigned char const *remap = *Remap;
    if (length > 0) {
        int count = length;
        do {
            unsigned short value = *depth_ptr;
            if (depth < value) {
                unsigned char color = *(unsigned char const *)source;
                if (color) {
                    unsigned int translated = table[*alpha];
                    *(unsigned short *)target = Palette[translated | remap[color]];
                    *depth_ptr = (unsigned short)depth;
                }
            }
            source = (unsigned char const *)source + 1;
            target = (unsigned short *)target + 1;
            depth_ptr++;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        } while (--count);
    }
}


// Depth-test each source pixel before reading its index and translating through the alpha row.
template <>
void BlitTransLucent75AlphaZReadWrite<unsigned short>::BlitForward(void *target, void const *source, int length,
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
            unsigned short value = *depth_ptr;
            if (depth < value) {
                unsigned char color = *(unsigned char const *)source;
                if (color) {
                    unsigned int translated = table[*alpha];
                    *(unsigned short *)target = ((Palette[translated | color] >> 2) & ChannelMask) + ((*(unsigned short *)target >> 2) & ChannelMask) * 3;
                    *depth_ptr = (unsigned short)depth;
                }
            }
            source = (unsigned char const *)source + 1;
            target = (unsigned short *)target + 1;
            depth_ptr++;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
            alpha++;
            alpha = AlphaBuffer->Wrap_Pointer(alpha);
        } while (--count);
    }
}


// Depth-test each source pixel before reading its index and translating through the alpha row.
template <>
void BlitTransLucent50AlphaZReadWrite<unsigned short>::BlitForward(void *target, void const *source, int length,
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
            unsigned short value = *depth_ptr;
            if (depth < value) {
                unsigned char color = *(unsigned char const *)source;
                if (color) {
                    unsigned int translated = table[*alpha];
                    *(unsigned short *)target = ((Palette[translated | color] >> 1) & ChannelMask) + ((*(unsigned short *)target >> 1) & ChannelMask);
                    *depth_ptr = (unsigned short)depth;
                }
            }
            source = (unsigned char const *)source + 1;
            target = (unsigned short *)target + 1;
            depth_ptr++;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
            alpha++;
            alpha = AlphaBuffer->Wrap_Pointer(alpha);
        } while (--count);
    }
}


// Depth-test each source pixel before reading its index and translating through the alpha row.
template <>
void BlitTransLucent25AlphaZReadWrite<unsigned short>::BlitForward(void *target, void const *source, int length,
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
            unsigned short value = *depth_ptr;
            if (depth < value) {
                unsigned char color = *(unsigned char const *)source;
                if (color) {
                    unsigned int translated = table[*alpha];
                    *(unsigned short *)target = ((Palette[translated | color] >> 2) & ChannelMask) * 3 + ((*(unsigned short *)target >> 2) & ChannelMask);
                    *depth_ptr = (unsigned short)depth;
                }
            }
            source = (unsigned char const *)source + 1;
            target = (unsigned short *)target + 1;
            depth_ptr++;
            depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
            alpha++;
            alpha = AlphaBuffer->Wrap_Pointer(alpha);
        } while (--count);
    }
}


// Skip the clipped left portion of an RLE row, then translate its visible nonzero pixels.
template <>
void RLEBlitTransXlat<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                                          int, void *, void *, int, int, void *) const
{
    int pixels_to_skip = skip;
    unsigned short *dest = (unsigned short *)target;
    unsigned char const *input = (unsigned char const *)source;
    if (pixels_to_skip > 0) {
        do {
            if (*input++ == 0) {
                pixels_to_skip -= *input++;
            } else {
                pixels_to_skip--;
            }
        } while (pixels_to_skip > 0);
        pixels_to_skip = -pixels_to_skip;
        length -= pixels_to_skip;
        dest += pixels_to_skip;
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
        } else {
            *dest++ = Palette[color];
            length--;
        }
    }
}


// Skip the clipped left portion of an RLE row, then translate its visible nonzero pixels.
template <>
void RLEBlitTransZRemapXlat<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                                          int, void *, void *, int, int, void *) const
{
    int pixels_to_skip = skip;
    unsigned short *dest = (unsigned short *)target;
    unsigned char const *input = (unsigned char const *)source;
    unsigned char const *remap = *RemapTable;
    if (pixels_to_skip > 0) {
        pixels_to_skip = Skip_RLE_Pixels(input, pixels_to_skip);
        pixels_to_skip = -pixels_to_skip;
        dest += pixels_to_skip;
        length -= pixels_to_skip;
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
        } else {
            *dest++ = Palette[remap[color]];
            length--;
        }
    }
}


// Skip the clipped left portion of an RLE row, then translate its visible nonzero pixels.
template <>
void RLEBlitTransDarken<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                                          int, void *, void *, int, int, void *) const
{
    int pixels_to_skip = skip;
    unsigned short *dest = (unsigned short *)target;
    unsigned char const *input = (unsigned char const *)source;
    if (pixels_to_skip > 0) {
        do {
            if (*input++ == 0) {
                pixels_to_skip -= *input++;
            } else {
                pixels_to_skip--;
            }
        } while (pixels_to_skip > 0);
        pixels_to_skip = -pixels_to_skip;
        length -= pixels_to_skip;
        dest += pixels_to_skip;
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
        } else {
            *dest = ((*dest >> 1) & ChannelMask);
            length--;
            ++dest;
        }
    }
}


// Skip the clipped left portion of an RLE row, then translate its visible nonzero pixels.
template <>
void RLEBlitTransLucent75<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                                          int, void *, void *, int, int, void *) const
{
    int pixels_to_skip = skip;
    unsigned short *dest = (unsigned short *)target;
    unsigned char const *input = (unsigned char const *)source;
    if (pixels_to_skip > 0) {
        do {
            if (*input++ == 0) {
                pixels_to_skip -= *input++;
            } else {
                pixels_to_skip--;
            }
        } while (pixels_to_skip > 0);
        pixels_to_skip = -pixels_to_skip;
        length -= pixels_to_skip;
        dest += pixels_to_skip;
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
        } else {
            *dest = ((Palette[color] >> 2) & ChannelMask) + ((*dest >> 2) & ChannelMask) * 3;
            ++dest;
            length--;
        }
    }
}


// Skip the clipped left portion of an RLE row, then translate its visible nonzero pixels.
template <>
void RLEBlitTransLucent50<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                                          int, void *, void *, int, int, void *) const
{
    int pixels_to_skip = skip;
    unsigned short *dest = (unsigned short *)target;
    unsigned char const *input = (unsigned char const *)source;
    if (pixels_to_skip > 0) {
        do {
            if (*input++ == 0) {
                pixels_to_skip -= *input++;
            } else {
                pixels_to_skip--;
            }
        } while (pixels_to_skip > 0);
        pixels_to_skip = -pixels_to_skip;
        dest += pixels_to_skip;
        length -= pixels_to_skip;
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
        } else {
            *dest = ((Palette[color] >> 1) & ChannelMask) + ((*dest >> 1) & ChannelMask);
            length--;
            ++dest;
        }
    }
}


// Skip the clipped left portion of an RLE row, then translate its visible nonzero pixels.
template <>
void RLEBlitTransLucent25<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                                          int, void *, void *, int, int, void *) const
{
    int pixels_to_skip = skip;
    unsigned short *dest = (unsigned short *)target;
    unsigned char const *input = (unsigned char const *)source;
    if (pixels_to_skip > 0) {
        do {
            if (*input++ == 0) {
                pixels_to_skip -= *input++;
            } else {
                pixels_to_skip--;
            }
        } while (pixels_to_skip > 0);
        pixels_to_skip = -pixels_to_skip;
        length -= pixels_to_skip;
        dest += pixels_to_skip;
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
        } else {
            *dest = ((Palette[color] >> 2) & ChannelMask) * 3 + ((*dest >> 2) & ChannelMask);
            ++dest;
            length--;
        }
    }
}


// Decode visible RLE pixels after a signed adjustment/depth test; no depth write.
template <>
void RLEBlitTransXlatZRead<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int, void *adjustment) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    char const *delta = (char const *)adjustment;
    unsigned short *output=(unsigned short *)target;
    unsigned char const *input = (unsigned char const *)source;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned short *)output + color;
            depth_ptr += color;
            delta += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                *(unsigned short *)output = Palette[color];
                ++output;
                --length;
            } else {
                ++output; --length;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Snapshot the shared remap once, then translate visible RLE pixels after a depth test.
// Opaque reads consume one signed adjustment byte; zero runs advance all three cursors.
template <>
void RLEBlitTransZRemapXlatZRead<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int, void *adjustment) const
{
    unsigned char const *input = (unsigned char const *)source;
    char const *delta = (char const *)adjustment;
    unsigned short *output=(unsigned short *)target;
    unsigned char const *remap=*RemapTable;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned short *)output + color;
            delta += color;
            depth_ptr += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                *(unsigned short *)output = Palette[remap[color]];
                ++output;
                --length;
            } else {
                ++output; --length;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Darken packed channels of visible RLE pixels after a signed adjustment/depth test.
template <>
void RLEBlitTransDarkenZRead<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int, void *adjustment) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned short *output=(unsigned short *)target;
    char const *delta = (char const *)adjustment;
    unsigned char const *input = (unsigned char const *)source;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned short *)output + color;
            delta += color;
            depth_ptr += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                *(unsigned short *)output = ((*(unsigned short *)output >> 1) & ChannelMask);
                --length;
                ++output;
            } else {
                --length; ++output;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Blend packed RLE pixels after a signed adjustment/depth test; no depth writes.
// Zero runs advance output, adjustment and depth; left clipping leaves adjustment unchanged.
template <>
void RLEBlitTransLucent75ZRead<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int, void *adjustment) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned short *output=(unsigned short *)target;
    char const *delta = (char const *)adjustment;
    unsigned char const *input = (unsigned char const *)source;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned short *)output + color;
            delta += color;
            depth_ptr += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                unsigned short previous=*output;
                *output++ = ((Palette[color] >> 2) & ChannelMask) + ((previous >> 2) & ChannelMask) * 3;
                --length;
            } else {
                ++output; --length;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Blend packed RLE pixels after a signed adjustment/depth test; no depth writes.
// Zero runs advance output, adjustment and depth; left clipping leaves adjustment unchanged.
template <>
void RLEBlitTransLucent50ZRead<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int, void *adjustment) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned short *output=(unsigned short *)target;
    char const *delta = (char const *)adjustment;
    unsigned char const *input = (unsigned char const *)source;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned short *)output + color;
            delta += color;
            depth_ptr += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                *(unsigned short *)output = ((Palette[color] >> 1) & ChannelMask) + ((*(unsigned short *)output >> 1) & ChannelMask);
                --length;
                ++output;
            } else {
                --length; ++output;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Blend packed RLE pixels after a signed adjustment/depth test; no depth writes.
// Zero runs advance output, adjustment and depth; left clipping leaves adjustment unchanged.
template <>
void RLEBlitTransLucent25ZRead<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int, void *adjustment) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned short *output=(unsigned short *)target;
    char const *delta = (char const *)adjustment;
    unsigned char const *input = (unsigned char const *)source;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned short *)output + color;
            delta += color;
            depth_ptr += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                unsigned short previous=*output;
                *output++ = ((Palette[color] >> 2) & ChannelMask) * 3 + ((previous >> 2) & ChannelMask);
                --length;
            } else {
                ++output; --length;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Blend packed RLE pixels after a signed adjustment/depth test; no depth writes.
// Zero runs advance output, adjustment and depth; left clipping leaves adjustment unchanged.
template <>
void RLEBlitTransLucent75ZReadWarp<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int warp_offset, void *adjustment) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned short *output=(unsigned short *)target;
    char const *delta = (char const *)adjustment;
    unsigned char const *input = (unsigned char const *)source;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned short *)output + color;
            delta += color;
            depth_ptr += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                unsigned short previous=output[warp_offset];
                *output++ = ((Palette[color] >> 2) & ChannelMask) + ((previous >> 2) & ChannelMask) * 3;
                --length;
            } else {
                ++output; --length;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Blend packed RLE pixels after a signed adjustment/depth test; no depth writes.
// Zero runs advance output, adjustment and depth; left clipping leaves adjustment unchanged.
template <>
void RLEBlitTransLucent50ZReadWarp<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int warp_offset, void *adjustment) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned short *output=(unsigned short *)target;
    char const *delta = (char const *)adjustment;
    unsigned char const *input = (unsigned char const *)source;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned short *)output + color;
            delta += color;
            depth_ptr += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                *(unsigned short *)output = ((Palette[color] >> 1) & ChannelMask) + ((((unsigned short *)output)[warp_offset] >> 1) & ChannelMask);
                --length;
                ++output;
            } else {
                --length; ++output;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Blend packed RLE pixels after a signed adjustment/depth test; no depth writes.
// Zero runs advance output, adjustment and depth; left clipping leaves adjustment unchanged.
template <>
void RLEBlitTransLucent25ZReadWarp<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int warp_offset, void *adjustment) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned short *output=(unsigned short *)target;
    char const *delta = (char const *)adjustment;
    unsigned char const *input = (unsigned char const *)source;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned short *)output + color;
            delta += color;
            depth_ptr += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                unsigned short previous=output[warp_offset];
                *output++ = ((Palette[color] >> 2) & ChannelMask) * 3 + ((previous >> 2) & ChannelMask);
                --length;
            } else {
                ++output; --length;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Decode an RLE row, depth-test each opaque pixel with its signed adjustment,
// and write depth after the pixel store. The depth cursor wraps in its ring.
template <>
void RLEBlitTransXlatZReadWrite<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                                                    int depth, void *depth_buffer, void *buffer, int level, int, void *adjustments) const
{
    unsigned char const *input = (unsigned char const *)source;
    unsigned short *dest = (unsigned short *)target;
    char const *delta = (char const *)adjustments;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (skip > 0) {
        skip=Skip_RLE_Pixels(input,skip);
        skip = -skip;
        dest += skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        length -= skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
            delta += color;
            depth_ptr += color;
        } else {
            Write_Depth_Checked_Pixel(dest, depth_ptr, delta, depth, color, length);
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Snapshot the shared remap once, then translate visible RLE pixels after a depth test.
// Opaque writes consume two signed adjustment bytes; zero runs consume one per skipped pixel.
template <>
void RLEBlitTransZRemapXlatZReadWrite<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int, void *adjustment) const
{
    unsigned char const *input = (unsigned char const *)source;
    char const *delta = (char const *)adjustment;
    unsigned short *output=(unsigned short *)target;
    unsigned char const *remap=*RemapTable;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned short *)output + color;
            delta += color;
            depth_ptr += color;
        } else {
            if (depth - *delta++ < *depth_ptr) {
                *output++ = Palette[remap[color]];
                *depth_ptr++=(unsigned short)(depth-*delta++);
                --length;
            } else {
                ++delta;
                output=(unsigned short *)output+1;
                ++depth_ptr; --length;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Decode an RLE row, depth-test each opaque pixel with its signed adjustment,
// and write depth after the pixel store. The depth cursor wraps in its ring.
template <>
void RLEBlitTransDarkenZReadWrite<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                                                    int depth, void *depth_buffer, void *buffer, int level, int, void *adjustments) const
{
    unsigned char const *input = (unsigned char const *)source;
    unsigned short *dest = (unsigned short *)target;
    char const *delta = (char const *)adjustments;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    if (skip > 0) {
        skip=Skip_RLE_Pixels(input,skip);
        skip = -skip;
        dest += skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        length -= skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
            delta += color;
            depth_ptr += color;
        } else {
            Write_Depth_Checked_Pixel(dest, depth_ptr, delta, depth, color, length);
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Quarter-channel blend, depth test and successful depth write.
// Left clipping and transparent runs do not advance adjustments; zero runs advance only output and length.
template <>
void RLEBlitTransLucent75ZReadWrite<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int, void *adjustment) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned short *output=(unsigned short *)target;
    char const *delta = (char const *)adjustment;
    unsigned char const *input = (unsigned char const *)source;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned short *)output + color;
        } else {
            if (depth - *delta < *depth_ptr) {
                unsigned short previous=*output;
                *output++ = ((Palette[color] >> 2) & ChannelMask) + ((previous >> 2) & ChannelMask) * 3;
                *depth_ptr++=(unsigned short)(depth-*delta);
                ++delta; --length;
            } else {
                ++delta;
                output=(unsigned short *)output+1;
                ++depth_ptr; --length;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Half-blend depth-tested RLE pixels and write depth after each successful pixel store.
// Left clipping does not advance adjustments; transparent runs advance all three cursors.
template <>
void RLEBlitTransLucent50ZReadWrite<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int, void *adjustment) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned short *output=(unsigned short *)target;
    char const *delta = (char const *)adjustment;
    unsigned char const *input = (unsigned char const *)source;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned short *)output + color;
            delta += color;
            depth_ptr += color;
        } else {
            if (depth - *delta < *depth_ptr) {
                *(unsigned short *)output = ((Palette[color] >> 1) & ChannelMask) + ((*(unsigned short *)output >> 1) & ChannelMask);
                *depth_ptr++=(unsigned short)(depth-*delta);
                ++delta; --length;
                output=(unsigned short *)output+1;
            } else {
                ++delta;
                output=(unsigned short *)output+1;
                ++depth_ptr; --length;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// Quarter-channel blend, depth test and successful depth write.
// Left clipping and transparent runs do not advance adjustments; zero runs advance only output and length.
template <>
void RLEBlitTransLucent25ZReadWrite<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *, int, int, void *adjustment) const
{
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned short *output=(unsigned short *)target;
    char const *delta = (char const *)adjustment;
    unsigned char const *input = (unsigned char const *)source;
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output = (unsigned short *)output + color;
        } else {
            if (depth - *delta < *depth_ptr) {
                unsigned short previous=*output;
                *output++ = ((Palette[color] >> 2) & ChannelMask) * 3 + ((previous >> 2) & ChannelMask);
                *depth_ptr++=(unsigned short)(depth-*delta);
                ++delta; --length;
            } else {
                ++delta;
                output=(unsigned short *)output+1;
                ++depth_ptr; --length;
            }
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
    }
}


// RLE transparent translation with a selected alpha lookup row and a wrapping buffer.
template <>
void RLEBlitTransXlatAlpha<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                                                int, void *, void *buffer, int level, int, void *) const
{
    unsigned char const *input = (unsigned char const *)source;
    unsigned short *dest = (unsigned short *)target;
    unsigned short *alpha = (unsigned short *)buffer;
    int row = level > 0 ? level : 0;
    row = (row * 261) >> 11;
    if (row > 254) row = 254;
    unsigned short const *table = AlphaTable[row];
    if (skip > 0) {
        do {
            if (*input++ == 0) skip -= *input++;
            else skip--;
        } while (skip > 0);
        skip = -skip;
        length -= skip;
        alpha = (unsigned short *)buffer + skip;
        dest += skip;
        alpha = AlphaBuffer->Wrap_Pointer(alpha);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
            alpha += color;
        } else {
            unsigned int translated = table[*alpha];
            *dest++ = Palette[translated | color];
            length--;
            alpha++;
        }
        alpha = AlphaBuffer->Wrap_Pointer(alpha);
    }
}


// RLE transparent translation with a selected alpha lookup row and a wrapping buffer.
template <>
void RLEBlitTransZRemapXlatAlpha<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                                                int, void *, void *buffer, int level, int, void *) const
{
    unsigned char const *input = (unsigned char const *)source;
    unsigned short *dest = (unsigned short *)target;
    unsigned char const *remap = *RemapTable;
    unsigned short *alpha = (unsigned short *)buffer;
    int row = level > 0 ? level : 0;
    row = (row * 261) >> 11;
    if (row > 254) row = 254;
    unsigned short const *table = AlphaTable[row];
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        length -= skip;
        alpha = (unsigned short *)buffer + skip;
        dest += skip;
        alpha = AlphaBuffer->Wrap_Pointer(alpha);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
            alpha += color;
        } else {
            unsigned int translated = table[*alpha];
            *dest++ = Palette[translated | remap[color]];
            length--;
            alpha++;
        }
        alpha = AlphaBuffer->Wrap_Pointer(alpha);
    }
}


// RLE transparent translation with a selected alpha lookup row and a wrapping buffer.
template <>
void RLEBlitTransLucent75Alpha<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                                                int, void *, void *buffer, int level, int, void *) const
{
    unsigned char const *input = (unsigned char const *)source;
    unsigned short *dest = (unsigned short *)target;
    unsigned short *alpha = (unsigned short *)buffer;
    int row = level > 0 ? level : 0;
    row = (row * 261) >> 11;
    if (row > 254) row = 254;
    unsigned short const *table = AlphaTable[row];
    if (skip > 0) {
        do {
            if (*input++ == 0) skip -= *input++;
            else skip--;
        } while (skip > 0);
        skip = -skip;
        length -= skip;
        alpha = (unsigned short *)buffer + skip;
        dest += skip;
        alpha = AlphaBuffer->Wrap_Pointer(alpha);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
            alpha += color;
        } else {
            unsigned int translated = table[*alpha];
            *dest = ((Palette[translated | color] >> 2) & ChannelMask) + ((*dest >> 2) & ChannelMask) * 3;
            dest++;
            length--;
            alpha++;
        }
        alpha = AlphaBuffer->Wrap_Pointer(alpha);
    }
}


// RLE transparent translation with a selected alpha lookup row and a wrapping buffer.
template <>
void RLEBlitTransLucent50Alpha<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                                                int, void *, void *buffer, int level, int, void *) const
{
    unsigned char const *input = (unsigned char const *)source;
    unsigned short *dest = (unsigned short *)target;
    unsigned short *alpha = (unsigned short *)buffer;
    int row = level > 0 ? level : 0;
    row = (row * 261) >> 11;
    if (row > 254) row = 254;
    unsigned short const *table = AlphaTable[row];
    if (skip > 0) {
        do {
            if (*input++ == 0) skip -= *input++;
            else skip--;
        } while (skip > 0);
        skip = -skip;
        length -= skip;
        alpha = (unsigned short *)buffer + skip;
        dest += skip;
        alpha = AlphaBuffer->Wrap_Pointer(alpha);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
            alpha += color;
        } else {
            unsigned int translated = table[*alpha];
            *dest = ((Palette[translated | color] >> 1) & ChannelMask) + ((*dest >> 1) & ChannelMask);
            dest++;
            length--;
            alpha++;
        }
        alpha = AlphaBuffer->Wrap_Pointer(alpha);
    }
}


// RLE transparent translation with a selected alpha lookup row and a wrapping buffer.
template <>
void RLEBlitTransLucent25Alpha<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                                                int, void *, void *buffer, int level, int, void *) const
{
    unsigned char const *input = (unsigned char const *)source;
    unsigned short *dest = (unsigned short *)target;
    unsigned short *alpha = (unsigned short *)buffer;
    int row = level > 0 ? level : 0;
    row = (row * 261) >> 11;
    if (row > 254) row = 254;
    unsigned short const *table = AlphaTable[row];
    if (skip > 0) {
        do {
            if (*input++ == 0) skip -= *input++;
            else skip--;
        } while (skip > 0);
        skip = -skip;
        length -= skip;
        alpha = (unsigned short *)buffer + skip;
        dest += skip;
        alpha = AlphaBuffer->Wrap_Pointer(alpha);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
            alpha += color;
        } else {
            unsigned int translated = table[*alpha];
            *dest = ((Palette[translated | color] >> 2) & ChannelMask) * 3 + ((*dest >> 2) & ChannelMask);
            dest++;
            length--;
            alpha++;
        }
        alpha = AlphaBuffer->Wrap_Pointer(alpha);
    }
}


// Decode an RLE row, depth-test each opaque pixel with its signed adjustment,
// and translate it through the alpha row. Both auxiliary pointers wrap in their rings.
template <>
void RLEBlitTransXlatAlphaZRead<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                                                    int depth, void *depth_buffer, void *buffer, int level, int, void *adjustments) const
{
    unsigned char const *input = (unsigned char const *)source;
    char const *delta = (char const *)adjustments;
    unsigned short *dest = (unsigned short *)target;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned short *alpha = (unsigned short *)buffer;
    int row = level > 0 ? level : 0;
    row = (int)((unsigned int)row * 261u) >> 11;
    if (row > 254) row = 254;
    unsigned short const *table = AlphaTable[row];
    if (skip > 0) {
        skip=Skip_RLE_Pixels(input,skip);
        skip = -skip;
        dest += skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        length -= skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha += skip;
        alpha = AlphaBuffer->Wrap_Pointer(alpha);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
            delta += color;
            depth_ptr += color;
            alpha += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                unsigned int translated = table[*alpha];
                *dest++ = Palette[translated | color];
                --length;
            } else {
                ++dest; --length;
            }
            ++alpha;
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha = AlphaBuffer->Wrap_Pointer(alpha);
    }
}


// Decode an RLE row, depth-test each opaque pixel with its signed adjustment,
// and translate it through the alpha row. Both auxiliary pointers wrap in their rings.
template <>
void RLEBlitTransZRemapXlatAlphaZRead<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                                                    int depth, void *depth_buffer, void *buffer, int level, int, void *adjustments) const
{
    unsigned char const *input = (unsigned char const *)source;
    char const *delta = (char const *)adjustments;
    unsigned char const *remap=*RemapTable;
    unsigned short *dest = (unsigned short *)target;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned short *alpha = (unsigned short *)buffer;
    int row = level > 0 ? level : 0;
    row = (int)((unsigned int)row * 261u) >> 11;
    if (row > 254) row = 254;
    unsigned short const *table = AlphaTable[row];
    if (skip > 0) {
        skip=Skip_RLE_Pixels(input,skip);
        skip = -skip;
        dest += skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        length -= skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha += skip;
        alpha = AlphaBuffer->Wrap_Pointer(alpha);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
            delta += color;
            depth_ptr += color;
            alpha += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                unsigned int translated = table[*alpha];
                *dest++ = Palette[translated | remap[color]];
                --length;
            } else {
                ++dest; --length;
            }
            ++alpha;
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha = AlphaBuffer->Wrap_Pointer(alpha);
    }
}


// Blend alpha-translated RLE pixels after a signed adjustment/depth test.
// Zero runs advance output, adjustments, depth and alpha; left clipping leaves adjustments unchanged.
template <>
void RLEBlitTransLucent75AlphaZRead<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *buffer, int level, int, void *adjustment) const
{
    unsigned short *output=(unsigned short *)target;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned char const *input = (unsigned char const *)source;
    char const *delta = (char const *)adjustment;
    unsigned short *alpha=(unsigned short *)buffer;
    int row=level>0?level:0;
    row=(int)((unsigned int)row*261u)>>11;
    if(row>254)row=254;
    unsigned short const *table=AlphaTable[row];
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha=(unsigned short *)buffer+skip;
        alpha=AlphaBuffer->Wrap_Pointer(alpha);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output += color;
            delta += color;
            depth_ptr += color;
            alpha += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                unsigned short previous=*output;
                *output++ = ((Palette[table[*alpha] | color] >> 2) & ChannelMask) + ((previous >> 2) & ChannelMask) * 3;
                --length;
            } else {
                ++output; --length;
            }
            ++alpha;
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha=AlphaBuffer->Wrap_Pointer(alpha);
    }
}


// Blend alpha-translated RLE pixels after a signed adjustment/depth test.
// Zero runs advance output, adjustments, depth and alpha; left clipping leaves adjustments unchanged.
template <>
void RLEBlitTransLucent50AlphaZRead<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *buffer, int level, int, void *adjustment) const
{
    unsigned char const *input = (unsigned char const *)source;
    unsigned short *alpha=(unsigned short *)buffer;
    unsigned short *output=(unsigned short *)target;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    char const *delta = (char const *)adjustment;
    int row=level>0?level:0;
    row=(int)((unsigned int)row*261u)>>11;
    if(row>254)row=254;
    unsigned short const *table=AlphaTable[row];
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha=(unsigned short *)buffer+skip;
        alpha=AlphaBuffer->Wrap_Pointer(alpha);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output += color;
            delta += color;
            depth_ptr += color;
            alpha += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                *(unsigned short *)output = ((Palette[table[*alpha] | color] >> 1) & ChannelMask) + ((*(unsigned short *)output >> 1) & ChannelMask);
                --length;
                ++output;
            } else {
                --length; ++output;
            }
            ++alpha;
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha=AlphaBuffer->Wrap_Pointer(alpha);
    }
}


// Blend alpha-translated RLE pixels after a signed adjustment/depth test.
// Zero runs advance output, adjustments, depth and alpha; left clipping leaves adjustments unchanged.
template <>
void RLEBlitTransLucent25AlphaZRead<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *buffer, int level, int, void *adjustment) const
{
    unsigned short *output=(unsigned short *)target;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned char const *input = (unsigned char const *)source;
    char const *delta = (char const *)adjustment;
    unsigned short *alpha=(unsigned short *)buffer;
    int row=level>0?level:0;
    row=(int)((unsigned int)row*261u)>>11;
    if(row>254)row=254;
    unsigned short const *table=AlphaTable[row];
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha=(unsigned short *)buffer+skip;
        alpha=AlphaBuffer->Wrap_Pointer(alpha);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output += color;
            delta += color;
            depth_ptr += color;
            alpha += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                unsigned short previous=*output;
                *output++ = ((Palette[table[*alpha] | color] >> 2) & ChannelMask) * 3 + ((previous >> 2) & ChannelMask);
                --length;
            } else {
                ++output; --length;
            }
            ++alpha;
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha=AlphaBuffer->Wrap_Pointer(alpha);
    }
}


// Blend packed RLE pixels after a signed adjustment/depth test; no depth writes.
// Zero runs advance output, adjustment, depth and alpha; left clipping leaves adjustment unchanged.
template <>
void RLEBlitTransLucent75AlphaZReadWarp<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *buffer, int level, int warp_offset, void *adjustment) const
{
    unsigned short *output=(unsigned short *)target;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned char const *input = (unsigned char const *)source;
    char const *delta = (char const *)adjustment;
    unsigned short *alpha=(unsigned short *)buffer;
    int row=level>0?level:0;
    row=(int)((unsigned int)row*261u)>>11;
    if(row>254)row=254;
    unsigned short const *table=AlphaTable[row];
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha=(unsigned short *)buffer+skip;
        alpha=AlphaBuffer->Wrap_Pointer(alpha);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            delta += color;
            depth_ptr += color;
            output += color;
            alpha += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                unsigned short previous=output[warp_offset];
                *output++ = ((Palette[table[*alpha] | color] >> 2) & ChannelMask) + ((previous >> 2) & ChannelMask) * 3;
                --length;
            } else {
                ++output; --length;
            }
            ++alpha;
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha=AlphaBuffer->Wrap_Pointer(alpha);
    }
}


// Blend alpha-translated RLE pixels after a signed adjustment/depth test.
// Zero runs advance output, adjustments, depth and alpha; left clipping leaves adjustments unchanged.
template <>
void RLEBlitTransLucent50AlphaZReadWarp<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *buffer, int level, int warp_offset, void *adjustment) const
{
    unsigned char const *input = (unsigned char const *)source;
    unsigned short *alpha=(unsigned short *)buffer;
    unsigned short *output=(unsigned short *)target;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    char const *delta = (char const *)adjustment;
    int row=level>0?level:0;
    row=(int)((unsigned int)row*261u)>>11;
    if(row>254)row=254;
    unsigned short const *table=AlphaTable[row];
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha=(unsigned short *)buffer+skip;
        alpha=AlphaBuffer->Wrap_Pointer(alpha);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output += color;
            delta += color;
            depth_ptr += color;
            alpha += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                *(unsigned short *)output = ((Palette[table[*alpha] | color] >> 1) & ChannelMask) + ((((unsigned short *)output)[warp_offset] >> 1) & ChannelMask);
                --length;
                ++output;
            } else {
                --length; ++output;
            }
            ++alpha;
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha=AlphaBuffer->Wrap_Pointer(alpha);
    }
}


// Blend packed RLE pixels after a signed adjustment/depth test; no depth writes.
// Zero runs advance output, adjustment, depth and alpha; left clipping leaves adjustment unchanged.
template <>
void RLEBlitTransLucent25AlphaZReadWarp<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *buffer, int level, int warp_offset, void *adjustment) const
{
    unsigned short *output=(unsigned short *)target;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned char const *input = (unsigned char const *)source;
    char const *delta = (char const *)adjustment;
    unsigned short *alpha=(unsigned short *)buffer;
    int row=level>0?level:0;
    row=(int)((unsigned int)row*261u)>>11;
    if(row>254)row=254;
    unsigned short const *table=AlphaTable[row];
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha=(unsigned short *)buffer+skip;
        alpha=AlphaBuffer->Wrap_Pointer(alpha);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            delta += color;
            depth_ptr += color;
            output += color;
            alpha += color;
        } else {
            if (depth - *delta++ < *depth_ptr++) {
                unsigned short previous=output[warp_offset];
                *output++ = ((previous >> 2) & ChannelMask) + ((Palette[table[*alpha] | color] >> 2) & ChannelMask) * 3;
                --length;
            } else {
                ++output; --length;
            }
            ++alpha;
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha=AlphaBuffer->Wrap_Pointer(alpha);
    }
}


// Decode an RLE row, depth-test each opaque pixel with its signed adjustment,
// and translate it through the alpha row. Both auxiliary pointers wrap in their rings.
template <>
void RLEBlitTransXlatAlphaZReadWrite<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                                                    int depth, void *depth_buffer, void *buffer, int level, int, void *adjustments) const
{
    unsigned char const *input = (unsigned char const *)source;
    unsigned short *dest = (unsigned short *)target;
    unsigned short *alpha = (unsigned short *)buffer;
    char const *delta = (char const *)adjustments;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    int row = level > 0 ? level : 0;
    row = (int)((unsigned int)row * 261u) >> 11;
    if (row > 254) row = 254;
    unsigned short const *table = AlphaTable[row];
    if (skip > 0) {
        skip=Skip_RLE_Pixels(input,skip);
        skip = -skip;
        dest += skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        length -= skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha += skip;
        alpha = AlphaBuffer->Wrap_Pointer(alpha);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
            delta += color;
            depth_ptr += color;
            alpha += color;
        } else {
            if (depth - *delta < *depth_ptr) {
                unsigned int translated = table[*alpha];
                *dest++ = Palette[translated | color];
                *depth_ptr++ = (unsigned short)(depth - *delta);
                ++delta; --length;
            } else {
                ++delta; ++dest; ++depth_ptr; --length;
            }
            ++alpha;
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha = AlphaBuffer->Wrap_Pointer(alpha);
    }
}


// Decode an RLE row, depth-test each opaque pixel with its signed adjustment,
// and translate it through the alpha row. Both auxiliary pointers wrap in their rings.
template <>
void RLEBlitTransZRemapXlatAlphaZReadWrite<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                                                    int depth, void *depth_buffer, void *buffer, int level, int, void *adjustments) const
{
    unsigned char const *input = (unsigned char const *)source;
    unsigned char const *remap=*RemapTable;
    unsigned short *dest = (unsigned short *)target;
    unsigned short *alpha = (unsigned short *)buffer;
    char const *delta = (char const *)adjustments;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    int row = level > 0 ? level : 0;
    row = (int)((unsigned int)row * 261u) >> 11;
    if (row > 254) row = 254;
    unsigned short const *table = AlphaTable[row];
    if (skip > 0) {
        skip=Skip_RLE_Pixels(input,skip);
        skip = -skip;
        dest += skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        length -= skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha += skip;
        alpha = AlphaBuffer->Wrap_Pointer(alpha);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            dest += color;
            delta += color;
            depth_ptr += color;
            alpha += color;
        } else {
            if (depth - *delta < *depth_ptr) {
                unsigned int translated = table[*alpha];
                *dest++ = Palette[translated | remap[color]];
                *depth_ptr++ = (unsigned short)(depth - *delta);
                ++delta; --length;
            } else {
                ++delta; ++dest; ++depth_ptr; --length;
            }
            ++alpha;
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha = AlphaBuffer->Wrap_Pointer(alpha);
    }
}


// Blend alpha-translated RLE pixels after a signed adjustment/depth test.
// Zero runs advance output, adjustments, depth and alpha; left clipping leaves adjustments unchanged.
template <>
void RLEBlitTransLucent75AlphaZReadWrite<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *buffer, int level, int, void *adjustment) const
{
    unsigned short *output=(unsigned short *)target;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned char const *input = (unsigned char const *)source;
    char const *delta = (char const *)adjustment;
    unsigned short *alpha=(unsigned short *)buffer;
    int row=level>0?level:0;
    row=(int)((unsigned int)row*261u)>>11;
    if(row>254)row=254;
    unsigned short const *table=AlphaTable[row];
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha=(unsigned short *)buffer+skip;
        alpha=AlphaBuffer->Wrap_Pointer(alpha);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output += color;
            delta += color;
            depth_ptr += color;
            alpha += color;
        } else {
            if (depth - *delta < *depth_ptr) {
                unsigned short previous=*output;
                *output++ = ((Palette[table[*alpha] | color] >> 2) & ChannelMask) + ((previous >> 2) & ChannelMask) * 3;
                *depth_ptr++=(unsigned short)(depth-*delta);
                ++delta; --length;
            } else {
                ++delta;
                output=(unsigned short *)output+1;
                ++depth_ptr; --length;
            }
            ++alpha;
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha=AlphaBuffer->Wrap_Pointer(alpha);
    }
}


// Blend alpha-translated RLE pixels after a signed adjustment/depth test.
// Zero runs advance output, adjustments, depth and alpha; left clipping leaves adjustments unchanged.
template <>
void RLEBlitTransLucent50AlphaZReadWrite<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *buffer, int level, int, void *adjustment) const
{
    unsigned char const *input = (unsigned char const *)source;
    unsigned short *alpha=(unsigned short *)buffer;
    unsigned short *output=(unsigned short *)target;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    char const *delta = (char const *)adjustment;
    int row=level>0?level:0;
    row=(int)((unsigned int)row*261u)>>11;
    if(row>254)row=254;
    unsigned short const *table=AlphaTable[row];
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha=(unsigned short *)buffer+skip;
        alpha=AlphaBuffer->Wrap_Pointer(alpha);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output += color;
            delta += color;
            depth_ptr += color;
            alpha += color;
        } else {
            if (depth - *delta < *depth_ptr) {
                *(unsigned short *)output = ((Palette[table[*alpha] | color] >> 1) & ChannelMask) + ((*(unsigned short *)output >> 1) & ChannelMask);
                *depth_ptr++=(unsigned short)(depth-*delta);
                ++delta; --length;
                output=(unsigned short *)output+1;
            } else {
                ++delta;
                output=(unsigned short *)output+1;
                ++depth_ptr; --length;
            }
            ++alpha;
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha=AlphaBuffer->Wrap_Pointer(alpha);
    }
}


// Blend alpha-translated RLE pixels after a signed adjustment/depth test.
// Zero runs advance output, adjustments, depth and alpha; left clipping leaves adjustments unchanged.
template <>
void RLEBlitTransLucent25AlphaZReadWrite<unsigned short>::Blit(void *target, void const *source, int length, int skip,
                      int depth, void *depth_buffer, void *buffer, int level, int, void *adjustment) const
{
    unsigned short *output=(unsigned short *)target;
    unsigned short *depth_ptr = (unsigned short *)depth_buffer;
    unsigned char const *input = (unsigned char const *)source;
    char const *delta = (char const *)adjustment;
    unsigned short *alpha=(unsigned short *)buffer;
    int row=level>0?level:0;
    row=(int)((unsigned int)row*261u)>>11;
    if(row>254)row=254;
    unsigned short const *table=AlphaTable[row];
    if (skip > 0) {
        skip = Skip_RLE_Pixels(input, skip);
        skip = -skip;
        output = (unsigned short *)output + skip;
        length -= skip;
        depth_ptr = (unsigned short *)depth_buffer + skip;
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha=(unsigned short *)buffer+skip;
        alpha=AlphaBuffer->Wrap_Pointer(alpha);
    }
    while (length > 0) {
        unsigned char color = *input++;
        if (color == 0) {
            color = *input++;
            length -= color;
            output += color;
            delta += color;
            depth_ptr += color;
            alpha += color;
        } else {
            if (depth - *delta < *depth_ptr) {
                unsigned short previous=*output;
                *output++ = ((previous >> 2) & ChannelMask) + ((Palette[table[*alpha] | color] >> 2) & ChannelMask) * 3;
                *depth_ptr++=(unsigned short)(depth-*delta);
                ++delta; --length;
            } else {
                ++delta;
                output=(unsigned short *)output+1;
                ++depth_ptr; --length;
            }
            ++alpha;
        }
        depth_ptr = DepthBuffer->Wrap_Pointer(depth_ptr);
        alpha=AlphaBuffer->Wrap_Pointer(alpha);
    }
}


// BlitTransDarkenZReadWrite<unsigned short>::BlitBackward (0x00487B50): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransDarkenZReadWrite<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                             int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransLucent75ZReadWrite<unsigned short>::BlitBackward (0x00487C20): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransLucent75ZReadWrite<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                               int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransLucent50ZReadWrite<unsigned short>::BlitBackward (0x00487CF0): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransLucent50ZReadWrite<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                               int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransLucent25ZReadWrite<unsigned short>::BlitBackward (0x00487DC0): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransLucent25ZReadWrite<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                               int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitPlainXlatAlpha<unsigned short>::BlitBackward (0x00487E90): defined inline in shared/plain_xlat_alpha_blitter.hpp;
// emitted with the class's vtable.


// BlitTransXlatAlpha<unsigned short>::BlitBackward (0x00487F70): defined inline in shared/trans_xlat_alpha_blitter.hpp;
// emitted with the class's vtable.


// BlitTransZRemapXlatAlpha<unsigned short>::BlitBackward (0x00488060): defined inline in shared/trans_remap_xlat_alpha_blitter.hpp;
// emitted with the class's vtable.


// BlitTransLucent75Alpha<unsigned short>::BlitBackward (0x00488160): defined inline in shared/trans_lucent_alpha_blitter.hpp;
// emitted with the class's vtable.


// BlitTransLucent50Alpha<unsigned short>::BlitBackward (0x00488250): defined inline in shared/trans_lucent_alpha_blitter.hpp;
// emitted with the class's vtable.


// BlitTransLucent25Alpha<unsigned short>::BlitBackward (0x00488350): defined inline in shared/trans_lucent_alpha_blitter.hpp;
// emitted with the class's vtable.


// BlitTransXlatWriteAlpha<unsigned short>::BlitBackward (0x004883F0): defined inline in shared/xlat_alpha_blitter.hpp;
// emitted with the class's vtable.


// BlitTransXlatMultWriteAlpha<unsigned short>::BlitBackward (0x00488490): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransXlatMultWriteAlpha<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                               int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTranslucentWriteAlpha<unsigned short>::BlitBackward (0x004886C0): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTranslucentWriteAlpha<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                             int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTranslucent50NonzeroAlpha<unsigned short>::BlitBackward (0x00488780): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTranslucent50NonzeroAlpha<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                                 int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTranslucent50ZeroAlpha<unsigned short>::BlitBackward (0x00488840): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTranslucent50ZeroAlpha<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                              int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// BlitTransXlatAlphaZRead<unsigned short>::BlitBackward (0x00488950): defined inline in shared/trans_xlat_alpha_read_blitter.hpp;
// emitted with the class's vtable.


// BlitTransZRemapXlatAlphaZRead<unsigned short>::BlitBackward (0x00488A80): defined inline in shared/trans_xlat_alpha_read_blitter.hpp;
// emitted with the class's vtable.


// BlitTransLucent75AlphaZRead<unsigned short>::BlitBackward (0x00488BC0): defined inline in shared/trans_xlat_alpha_read_blitter.hpp;
// emitted with the class's vtable.


// BlitTransLucent50AlphaZRead<unsigned short>::BlitBackward (0x00488CF0): defined inline in shared/trans_xlat_alpha_read_blitter.hpp;
// emitted with the class's vtable.


// BlitTransLucent25AlphaZRead<unsigned short>::BlitBackward (0x00488E30): defined inline in shared/trans_xlat_alpha_read_blitter.hpp;
// emitted with the class's vtable.


// BlitTransLucent75AlphaZReadWarp<unsigned short>::BlitBackward (0x00488F70): defined inline in shared/trans_lucent_alpha_warp_blitter.hpp;
// emitted with the class's vtable.


// BlitTransLucent50AlphaZReadWarp<unsigned short>::BlitBackward (0x004890B0): defined inline in shared/trans_lucent_alpha_warp_blitter.hpp;
// emitted with the class's vtable.


// BlitTransLucent25AlphaZReadWarp<unsigned short>::BlitBackward (0x004891F0): defined inline in shared/trans_lucent_alpha_warp_blitter.hpp;
// emitted with the class's vtable.


// BlitTransXlatAlphaZReadWrite<unsigned short>::BlitBackward (0x00489310): defined inline in shared/trans_xlat_alpha_depth_blitter.hpp;
// emitted with the class's vtable.


// BlitTransZRemapXlatAlphaZReadWrite<unsigned short>::BlitBackward (0x00489420): defined inline in shared/trans_remap_xlat_alpha_depth_blitter.hpp;
// emitted with the class's vtable.


// BlitTransLucent75AlphaZReadWrite<unsigned short>::BlitBackward (0x00489560): defined inline in shared/trans_lucent_alpha_depth_blitter.hpp;
// emitted with the class's vtable.


// BlitTransLucent50AlphaZReadWrite<unsigned short>::BlitBackward (0x004896A0): defined inline in shared/trans_lucent_alpha_depth_blitter.hpp;
// emitted with the class's vtable.


// BlitTransLucent25AlphaZReadWrite<unsigned short>::BlitBackward (0x004897E0): defined inline in shared/trans_lucent_alpha_depth_blitter.hpp;
// emitted with the class's vtable.


// BlitTransXlat<unsigned short>'s scalar deleting destructor (0x0048C790): compiler-generated from the class's
// inline virtual destructor (shared/basic_xlat_blitters.hpp), emitted with its vtable.


// 0x0048c7b0: function not yet identified
//
// Owner: Blitter, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 32 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall scalar_deleting_destructor_0048c7b0(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0048c7b0 {

extern void  operator_delete_00781d3b(unsigned int);
extern "C" void* Blitter_vtable_0079e52c;

struct Owner_0048c7b0 { unsigned int * scalar_deleting_destructor_0048c7b0(unsigned char flags); };

unsigned int * Owner_0048c7b0::scalar_deleting_destructor_0048c7b0(unsigned char flags)
{
    unsigned int * self = (unsigned int *)this;
    *self = (unsigned int)(&Blitter_vtable_0079e52c);
    if ((flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(self));
    }
    return self;
}

}  // namespace seed_0048c7b0


// 0x0048c890: function not yet identified
//
// Owner: RLEBlitter, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 32 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall scalar_deleting_destructor_0048c890(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0048c890 {

extern void  operator_delete_00781d3b(unsigned int);
extern "C" void* RLEBlitter_vtable_0079e53c;

struct Owner_0048c890 { unsigned int * scalar_deleting_destructor_0048c890(unsigned char flags); };

unsigned int * Owner_0048c890::scalar_deleting_destructor_0048c890(unsigned char flags)
{
    unsigned int * self = (unsigned int *)this;
    *self = (unsigned int)(&RLEBlitter_vtable_0079e53c);
    if ((flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(self));
    }
    return self;
}

}  // namespace seed_0048c890


// BlitPlainXlatAlpha<unsigned short>'s scalar deleting destructor (0x0048CCF0): compiler-generated from the class's
// inline virtual destructor (shared/plain_xlat_alpha_blitter.hpp), emitted with its vtable.


// BlitTransXlatAlpha<unsigned short>'s scalar deleting destructor (0x0048CD30): compiler-generated from the class's
// inline virtual destructor (shared/trans_xlat_alpha_blitter.hpp), emitted with its vtable.


// BlitTransZRemapXlatAlpha<unsigned short>'s scalar deleting destructor (0x0048CD70): compiler-generated from the class's
// inline virtual destructor (shared/trans_remap_xlat_alpha_blitter.hpp), emitted with its vtable.


// BlitTransLucent75Alpha<unsigned short>'s scalar deleting destructor (0x0048CDB0): compiler-generated from the class's
// inline virtual destructor (shared/trans_lucent_alpha_blitter.hpp), emitted with its vtable.


// BlitTransLucent50Alpha<unsigned short>'s scalar deleting destructor (0x0048CDF0): compiler-generated from the class's
// inline virtual destructor (shared/trans_lucent_alpha_blitter.hpp), emitted with its vtable.


// BlitTransLucent25Alpha<unsigned short>'s scalar deleting destructor (0x0048CE30): compiler-generated from the class's
// inline virtual destructor (shared/trans_lucent_alpha_blitter.hpp), emitted with its vtable.


// BlitTransXlatWriteAlpha<unsigned short>'s scalar deleting destructor (0x0048CE70): compiler-generated from the class's
// inline virtual destructor (shared/xlat_alpha_blitter.hpp), emitted with its vtable.


// BlitTransXlatAlphaZRead<unsigned short>'s scalar deleting destructor (0x0048CF10): compiler-generated from the class's
// inline virtual destructor (shared/trans_xlat_alpha_read_blitter.hpp), emitted with its vtable.


// BlitTransZRemapXlatAlphaZRead<unsigned short>'s scalar deleting destructor (0x0048CF50): compiler-generated from the class's
// inline virtual destructor (shared/trans_xlat_alpha_read_blitter.hpp), emitted with its vtable.


// BlitTransLucent75AlphaZRead<unsigned short>'s scalar deleting destructor (0x0048CF90): compiler-generated from the class's
// inline virtual destructor (shared/trans_xlat_alpha_read_blitter.hpp), emitted with its vtable.


// BlitTransLucent50AlphaZRead<unsigned short>'s scalar deleting destructor (0x0048CFD0): compiler-generated from the class's
// inline virtual destructor (shared/trans_xlat_alpha_read_blitter.hpp), emitted with its vtable.


// BlitTransLucent25AlphaZRead<unsigned short>'s scalar deleting destructor (0x0048D010): compiler-generated from the class's
// inline virtual destructor (shared/trans_xlat_alpha_read_blitter.hpp), emitted with its vtable.


// BlitTransLucent75AlphaZReadWarp<unsigned short>'s scalar deleting destructor (0x0048D050): compiler-generated from the class's
// inline virtual destructor (shared/trans_lucent_alpha_warp_blitter.hpp), emitted with its vtable.


// BlitTransLucent50AlphaZReadWarp<unsigned short>'s scalar deleting destructor (0x0048D090): compiler-generated from the class's
// inline virtual destructor (shared/trans_lucent_alpha_warp_blitter.hpp), emitted with its vtable.


// BlitTransLucent25AlphaZReadWarp<unsigned short>'s scalar deleting destructor (0x0048D0D0): compiler-generated from the class's
// inline virtual destructor (shared/trans_lucent_alpha_warp_blitter.hpp), emitted with its vtable.


// BlitTransXlatAlphaZReadWrite<unsigned short>'s scalar deleting destructor (0x0048D110): compiler-generated from the class's
// inline virtual destructor (shared/trans_xlat_alpha_depth_blitter.hpp), emitted with its vtable.


// BlitTransZRemapXlatAlphaZReadWrite<unsigned short>'s scalar deleting destructor (0x0048D150): compiler-generated from the class's
// inline virtual destructor (shared/trans_remap_xlat_alpha_depth_blitter.hpp), emitted with its vtable.


// BlitTransLucent75AlphaZReadWrite<unsigned short>'s scalar deleting destructor (0x0048D190): compiler-generated from the class's
// inline virtual destructor (shared/trans_lucent_alpha_depth_blitter.hpp), emitted with its vtable.


// BlitTransLucent50AlphaZReadWrite<unsigned short>'s scalar deleting destructor (0x0048D1D0): compiler-generated from the class's
// inline virtual destructor (shared/trans_lucent_alpha_depth_blitter.hpp), emitted with its vtable.


// BlitTransLucent25AlphaZReadWrite<unsigned short>'s scalar deleting destructor (0x0048D210): compiler-generated from the class's
// inline virtual destructor (shared/trans_lucent_alpha_depth_blitter.hpp), emitted with its vtable.


// RLEBlitTransXlat<unsigned short>'s scalar deleting destructor (0x0048D250): compiler-generated from the class's
// inline virtual destructor (shared/rle_blitter.hpp), emitted with its vtable.


// RLEBlitTransXlatAlpha<unsigned short>'s scalar deleting destructor (0x0048D4F0): compiler-generated from the class's
// inline virtual destructor (shared/rle_blitter.hpp), emitted with its vtable.


// RLEBlitTransZRemapXlatAlpha<unsigned short>'s scalar deleting destructor (0x0048D530): compiler-generated from the class's
// inline virtual destructor (shared/rle_blitter.hpp), emitted with its vtable.


// RLEBlitTransLucent75Alpha<unsigned short>'s scalar deleting destructor (0x0048D570): compiler-generated from the class's
// inline virtual destructor (shared/rle_blitter.hpp), emitted with its vtable.


// RLEBlitTransLucent50Alpha<unsigned short>'s scalar deleting destructor (0x0048D5B0): compiler-generated from the class's
// inline virtual destructor (shared/rle_blitter.hpp), emitted with its vtable.


// RLEBlitTransLucent25Alpha<unsigned short>'s scalar deleting destructor (0x0048D5F0): compiler-generated from the class's
// inline virtual destructor (shared/rle_blitter.hpp), emitted with its vtable.


// RLEBlitTransXlatAlphaZRead<unsigned short>'s scalar deleting destructor (0x0048D630): compiler-generated from the class's
// inline virtual destructor (shared/rle_alpha_depth.hpp), emitted with its vtable.


// RLEBlitTransZRemapXlatAlphaZRead<unsigned short>'s scalar deleting destructor (0x0048D670): compiler-generated from the class's
// inline virtual destructor (shared/rle_active_alpha_depth.hpp), emitted with its vtable.


// RLEBlitTransLucent75AlphaZRead<unsigned short>'s scalar deleting destructor (0x0048D6B0): compiler-generated from the class's
// inline virtual destructor (shared/rle_alpha_blend_depth.hpp), emitted with its vtable.


// RLEBlitTransLucent50AlphaZRead<unsigned short>'s scalar deleting destructor (0x0048D6F0): compiler-generated from the class's
// inline virtual destructor (shared/rle_alpha_blend_depth.hpp), emitted with its vtable.


// RLEBlitTransLucent25AlphaZRead<unsigned short>'s scalar deleting destructor (0x0048D730): compiler-generated from the class's
// inline virtual destructor (shared/rle_alpha_blend_depth.hpp), emitted with its vtable.


// RLEBlitTransLucent75AlphaZReadWarp<unsigned short>'s scalar deleting destructor (0x0048D770): compiler-generated from the class's
// inline virtual destructor (shared/rle_alpha_blend_depth.hpp), emitted with its vtable.


// RLEBlitTransLucent50AlphaZReadWarp<unsigned short>'s scalar deleting destructor (0x0048D7B0): compiler-generated from the class's
// inline virtual destructor (shared/rle_alpha_blend_depth.hpp), emitted with its vtable.


// RLEBlitTransLucent25AlphaZReadWarp<unsigned short>'s scalar deleting destructor (0x0048D7F0): compiler-generated from the class's
// inline virtual destructor (shared/rle_alpha_blend_depth.hpp), emitted with its vtable.


// RLEBlitTransXlatAlphaZReadWrite<unsigned short>'s scalar deleting destructor (0x0048D830): compiler-generated from the class's
// inline virtual destructor (shared/rle_alpha_depth.hpp), emitted with its vtable.


// RLEBlitTransZRemapXlatAlphaZReadWrite<unsigned short>'s scalar deleting destructor (0x0048D870): compiler-generated from the class's
// inline virtual destructor (shared/rle_active_alpha_depth.hpp), emitted with its vtable.


// RLEBlitTransLucent75AlphaZReadWrite<unsigned short>'s scalar deleting destructor (0x0048D8B0): compiler-generated from the class's
// inline virtual destructor (shared/rle_alpha_blend_depth.hpp), emitted with its vtable.


// RLEBlitTransLucent50AlphaZReadWrite<unsigned short>'s scalar deleting destructor (0x0048D8F0): compiler-generated from the class's
// inline virtual destructor (shared/rle_alpha_blend_depth.hpp), emitted with its vtable.


// RLEBlitTransLucent25AlphaZReadWrite<unsigned short>'s scalar deleting destructor (0x0048D930): compiler-generated from the class's
// inline virtual destructor (shared/rle_alpha_blend_depth.hpp), emitted with its vtable.


// BlitTransXlat<unsigned short>::BlitBackward (0x00486210): the backward entry forwards through the virtual forward
// slot, with zero as the last argument.
template <>
void BlitTransXlat<unsigned short>::BlitBackward(void *target, void const *source, int length,
                                                 int a, void *b, void *c, int d) const
{
    BlitForward(target, source, length, a, b, c, d, 0);
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<ConvertClass *>::Clear (0x00486240; DynamicVectorClass<ConvertClass *> vtable 0x0079df30 slot 3).
// DynamicVectorClass<ConvertClass *>::ID_pointer (0x00486270; DynamicVectorClass<ConvertClass *> vtable 0x0079df30 slot 5).
// VectorClass<ConvertClass *>::Fetch (0x00486290; VectorClass<ConvertClass *> vtable 0x0079df50 slot 6).
// VectorClass<ConvertClass *>::operator equal (0x0048D970; VectorClass<ConvertClass *> vtable 0x0079df50 slot 1).
// VectorClass<ConvertClass *>::Resize (0x0048D9B0; VectorClass<ConvertClass *> vtable 0x0079df50 slot 2).
// VectorClass<ConvertClass *>::Clear (0x0048DA60; VectorClass<ConvertClass *> vtable 0x0079df50 slot 3).
// VectorClass<ConvertClass *>::ID_object (0x0048DA90; VectorClass<ConvertClass *> vtable 0x0079df50 slot 4).
// VectorClass<ConvertClass *>::ID_pointer (0x0048DAD0; VectorClass<ConvertClass *> vtable 0x0079df50 slot 5).
// DynamicVectorClass<ConvertClass *>::Resize (0x0048DAF0; DynamicVectorClass<ConvertClass *> vtable 0x0079df30 slot 2).
// DynamicVectorClass<ConvertClass *>::ID_object (0x0048DBA0; DynamicVectorClass<ConvertClass *> vtable 0x0079df30 slot 4).


extern "C" unsigned char vtable_48dbd0[]; extern "C" void __cdecl release_buffer_48dbd0(void*); extern "C" void __cdecl release_object_48dbd0(void*);
class Leaf_48dbd0 { public: void* get(unsigned int flags); };
void* Leaf_48dbd0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_48dbd0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_48dbd0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_48dbd0(this); return this; }


extern "C" unsigned char vtable_48dc20[]; extern "C" void __cdecl release_buffer_48dc20(void*); extern "C" void __cdecl release_object_48dc20(void*);
class Leaf_48dc20 { public: void* get(unsigned int flags); };
void* Leaf_48dc20::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_48dc20; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_48dc20((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_48dc20(this); return this; }
