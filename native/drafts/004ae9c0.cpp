// NON-EXACT DRAFT 0x4AE9C0 FUN_004ae9c0
// entry 0x4AE9C0; exact-match score 99.44 (misses_index best), this text 90.75; plan "draft as-is"
// origin lane: Scout795 (build/class-work/Scout795/w/t2.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1069): definition named Draw_Trail_Line_4AE9C0 (the tree's declaration); each changed line is marked NATIVE.
// RA2 trail blend at004AE9C0, authored from exact target and current accepted sibling.
#include "shared/blit.hpp"
#include "shared/crt_memory.hpp"
#include "shared/dsurface.hpp"
#include "shared/fast_math.hpp"
#include "shared/hicolor.hpp"
#include "shared/rgb_class.hpp"
// @ref ?Clip_Line@@YI_NAAUPoint2D@@0ABVRect@@@Z=00775620
// @ref ?Intersect@@YI?AVRect@@ABV1@0PAH1@Z=0041fdf0
// @ref ?Fast_Sqrt@@YAMN@Z=004ba410
// @ref ?Get_Buffer_Ptr@ABuffer@@QAEPAGUPoint2D@@@Z=00411230
// @ref ?Get_Buffer_Ptr@ZBuffer@@QAEPAGUPoint2D@@@Z=00776340
// @ref ?AlphaBuffer@@3PAVABuffer@@A=008315b4
// @ref ?DepthBuffer@@3PAVZBuffer@@A=00839c6c
// @ref ?Brighten_Hicolor_Pixel@@YIHABVRGBClass@@HHH@Z=004ae8d0
// @ref ?unknown_shift_852fb8@@3HA=00852fb8
// @ref ?unknown_shift_852fbc@@3HA=00852fbc
// @ref ?unknown_shift_852fc0@@3HA=00852fc0
// @ref ?unknown_shift_852fc4@@3HA=00852fc4
// @ref ?unknown_shift_852fc8@@3HA=00852fc8
// @ref ?unknown_shift_852fcc@@3HA=00852fcc
// Actual source/background blend, followed by the ring brightness scale.
static inline unsigned short Trail_Blend_Pixel(unsigned short pixel, int level, int inverse, int red, int green, int blue)
{
    RGBClass const &background = Deconstruct_Hicolor_Pixel(pixel);
    return (unsigned short)Build_Hicolor_Pixel(
        ((((background.Red * inverse) >> 8) + red) * level) >> 7,
        ((((background.Green * inverse) >> 8) + green) * level) >> 7,
        ((((background.Blue * inverse) >> 8) + blue) * level) >> 7);
}

bool DSurface::Draw_Trail_Line_4AE9C0(Rect const & cliprect, Point2D const & startpoint, Point2D const & endpoint, RGBClass const & color, int intensity, int start_depth, int end_depth)
{
    if (intensity >= 8) {
    Point2D start;
    Point2D end;
    Rect rect = cliprect.Intersect(Get_Rect());
    if (rect.Width != 0 || rect.Height != 0) {
    start = Offset_Point(startpoint, rect);
    end = Offset_Point(endpoint, rect);
    int z1 = start_depth;
    int z2 = end_depth;
    if (start.X > end.X) {
        Point2D temp = start;
        start = end;
        end = temp;
        z1 = end_depth;
        z2 = start_depth;
    }
    int px = start.X;
    int py = start.Y;
    Point2D origend = end;
    if (Clip_Line(start, end, rect)) {
    int zend = z2;
    int zstart = z1;
    if (end.X != origend.X || end.Y != origend.Y) {
        Point2D clipped_point = Point2D(px, py) - end;
        Point2D full_point = Point2D(px, py) - origend;
        int full = full_point.Length();
        int clipped = clipped_point.Length();
        unsigned int amount = abs((int)((double)clipped / (double)full * (double)(z1 - z2)));
        if (z1 < z2) {
            zend = z1 + amount;
        } else {
            zend = z1 - amount;
        }
    }
    if (start.X != px || start.Y != py) {
        Point2D clipped_point = start - origend;
        Point2D full_point = Point2D(px, py) - origend;
        int full = full_point.Length();
        int clipped = clipped_point.Length();
        unsigned int amount = abs((int)((double)clipped / (double)full * (double)(z1 - z2)));
        if (z2 > z1) {
            zstart = z2 - amount;
        } else {
            zstart = z2 + amount;
        }
    }
    Bytes_Per_Pixel();
    void * buffer = Lock(start);
    if (DepthBuffer != 0) {
    Point2D point = start;
    point.Y -= DepthBuffer->Area.Y;
    unsigned short * zbuffer = DepthBuffer->Get_Buffer_Ptr(point);
    point = start;
    point.Y -= AlphaBuffer->Area.Y;
    unsigned short * level = AlphaBuffer->Get_Buffer_Ptr(point);
    int start_y = start.Y;
    int zwrap = -1;
    ZBuffer *depth_buffer = DepthBuffer;
    unsigned short z = (unsigned short)(zstart + (short)(depth_buffer->Area.Y + depth_buffer->Get_Scroll()) - start_y - rect.Y);
    int zwidth = DepthBuffer->Width;
    if (buffer != 0) {
    int red = (color.Red * intensity) >> 8;
    int green = (color.Green * intensity) >> 8;
    int blue = (color.Blue * intensity) >> 8;
    int inverse = 256 - intensity;
    int dx = end.X - start.X;
    int dy = end.Y - start.Y;
    int dz = zend - zstart;
    int pitch = Stride();
    if (dy < 0) {
        pitch = -pitch;
        zwidth = -zwidth;
        zwrap = 1;
    }
    int abs_dy = abs(dy);
    int zstep = 1;
    if (dz < 0) {
        zstep = -1;
    }
    int abs_dz = abs(dz);
    int dx2 = 2 * dx;
    int dy2 = 2 * abs_dy;
    int dz2 = 2 * abs_dz;
    if (abs_dz > dx && abs_dz > abs_dy) {
        int xdelta = dy2 - abs_dz;
        int ydelta = dx2 - abs_dz;
        if (abs_dz > 0) {
            int offset = 0;
            for (int i = abs_dz; i != 0; i--) {
                int v = *level;
                if (z < *zbuffer && v != 0) {
                    *(unsigned short *)((char *)buffer + offset) = Trail_Blend_Pixel(*(unsigned short *)((char *)buffer + offset), v, inverse, red, green, blue);
                }
                if (xdelta > 0) {
                    buffer = (unsigned char *)buffer + pitch;
                    xdelta -= dz2;
                    zbuffer += zwidth;
                    if (zwidth > 0) {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    } else {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
                    }
                    level += zwidth;
                    if (zwidth > 0) {
                        level = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)level);
                    } else {
                        level = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)level);
                    }
                    z += zwrap;
                }
                z += zstep;
                if (ydelta > 0) {
                    zbuffer++;
                    offset += 2;
                    zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    level++;
                    level = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)level);
                    ydelta -= dz2;
                }
                ydelta += dx2;
                xdelta += dy2;
            }
        }
    } else if (dx > abs_dy) {
        int ydelta = dy2 - dx;
        int zdelta = dz2 - dx;
        int i = 0;
        if (dx > 0) {
            for (; i < dx; i++) {
                int v = *level;
                if (z < *zbuffer && v != 0) {
                    *(unsigned short *)((char *)buffer + 2 * i) = Trail_Blend_Pixel(*(unsigned short *)((char *)buffer + 2 * i), v, inverse, red, green, blue);
                }
                if (ydelta > 0) {
                    buffer = (unsigned char *)buffer + pitch;
                    ydelta -= 2 * dx;
                    zbuffer += zwidth;
                    if (zwidth > 0) {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    } else {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
                    }
                    z += zwrap;
                    level += zwidth;
                    if (zwidth > 0) {
                        level = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)level);
                    } else {
                        level = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)level);
                    }
                }
                if (zdelta > 0) {
                    z += zstep;
                    zdelta -= 2 * dx;
                }
                zbuffer++;
                ydelta += dy2;
                zdelta += dz2;
                zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                level++;
                level = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)level);
            }
        }
    } else {
        int xdelta = dx2 - abs_dy;
        int zdelta = 2 * abs_dz - abs_dy;
        if (abs_dy > 0) {
            int offset = 0;
            for (int i = abs_dy; i != 0; i--) {
                int v = *level;
                if (z < *zbuffer && v != 0) {
                    *(unsigned short *)((char *)buffer + offset) = Trail_Blend_Pixel(*(unsigned short *)((char *)buffer + offset), v, inverse, red, green, blue);
                }
                if (xdelta > 0) {
                    zbuffer++;
                    offset += 2;
                    zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    level++;
                    level = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)level);
                    xdelta -= dy2;
                }
                if (zdelta > 0) {
                    z += zstep;
                    zdelta -= dy2;
                }
                xdelta += dx2;
                zdelta += dz2;
                buffer = (unsigned char *)buffer + pitch;
                zbuffer += zwidth;
                if (zwidth > 0) {
                    zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                } else {
                    zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
                }
                z += zwrap;
                level += zwidth;
                if (zwidth > 0) {
                    level = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)level);
                } else {
                    level = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)level);
                }
            }
        }
    }
    Unlock();
    return(true);
    }
    }
    }
    }
    }
    return(false);
}
