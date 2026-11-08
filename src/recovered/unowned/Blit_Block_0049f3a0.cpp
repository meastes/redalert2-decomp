#include "shared/surface_blit.hpp"
#include "shared/convert.hpp"
#include "shared/blit.hpp"
void __fastcall unknown_blit_49f3a0(Surface &dest,void *converter,Surface &source,Rect const &source_rect,
                                  RectPoint2D const &point,Rect const &window,unsigned char const *remap,Blitter const *blitter,
                                  int height_offset,int gradient,int intensity,int unknown)
{
    ConvertClass *convert=static_cast<ConvertClass *>(converter);
    convert->ActiveRemap=remap;
    if(!blitter) blitter=convert->Select_Blitter_00485AA0(remap ? 16 : 0);
    Bit_Blit(dest,window,Rect(point.X,point.Y,source_rect.Width,source_rect.Height),source,source.Get_Rect(),source_rect,*blitter,height_offset,gradient,intensity,unknown);
}
