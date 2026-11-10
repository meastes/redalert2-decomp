// bool DSurface::Draw_Trail_Line_4AE9C0(...) (0x004AE9C0; LineTrail::Draw 0x005395C0): a line from startpoint to
// endpoint clipped to cliprect, blended in color at intensity over the pixels whose depth passes, the depth
// interpolated from start_depth to end_depth (z, x and y major Bresenham walks over the screen, depth and alpha
// buffers). The depth test reads the buffer behind DepthBuffer and the level the one behind AlphaBuffer; ra2-decomp's
// YR twin calls those DepthBuffer and AlphaBuffer the other way round (name lead, not settled here).
// From ra2-decomp 0x004BEAC0 (YR identical twin, src/dsurface.cpp DSurface::FUN_004BEAC0), on Scout795's draft
// (its blend helper kept; ra2-decomp's Alpha_Hicolor_Pixel form scores 80.75):
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
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
// @ref ?AlphaBuffer@@3PAVZBuffer@@A=008315b4
// @ref ?DepthBuffer@@3PAVZBuffer@@A=00839c6c
// @ref ?Brighten_Hicolor_Pixel@@YIHABVRGBClass@@HHH@Z=004ae8d0
// @ref ?unknown_shift_852fb8@@3HA=00852fb8
// @ref ?unknown_shift_852fbc@@3HA=00852fbc
// @ref ?unknown_shift_852fc0@@3HA=00852fc0
// @ref ?unknown_shift_852fc4@@3HA=00852fc4
// @ref ?unknown_shift_852fc8@@3HA=00852fc8
// @ref ?unknown_shift_852fcc@@3HA=00852fcc
// TibSun/ra2-decomp's buffer wrap helpers (zbuffer.h / abuffer.h): a pointer stepped past the end of the
// circular buffer comes back to its start.
static inline unsigned short *Blit_Wrap_Z_Buffer(unsigned short *buf)
{
    return((unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)buf));
}
static inline unsigned short *Blit_Wrap_A_Buffer(unsigned short *buf)
{
    return((unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)buf));
}
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
    Point2D start;
    Point2D end;
    if (intensity >= 8) {
    Rect rect = cliprect.Intersect(Get_Rect());
    if (rect.Width != 0 || rect.Height != 0) {
    start = Offset_Point(startpoint, rect);
    end = Offset_Point(endpoint, rect);
    int z2 = end_depth;
    int z1 = start_depth;
    if (start.X > end.X) {
        Point2D temp = start;
        start = end;
        end = temp;
        z1 = end_depth;
        z2 = start_depth;
    }
    Point2D origend = end;
    int px = start.X;
    int py = start.Y;
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
    unsigned short * abuffer = DepthBuffer->Get_Buffer_Ptr(point);
    point = start;
    point.Y -= AlphaBuffer->Area.Y;
    unsigned short * zbuffer = AlphaBuffer->Get_Buffer_Ptr(point);
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
                int v = *zbuffer;
                if (z < *abuffer && v != 0) {
                    *(unsigned short *)((char *)buffer + offset) = Trail_Blend_Pixel(*(unsigned short *)((char *)buffer + offset), v, inverse, red, green, blue);
                }
                if (xdelta > 0) {
                    buffer = (unsigned char *)buffer + pitch;
                    xdelta -= dz2;
                    abuffer += zwidth;
                    if (zwidth > 0) {
                        abuffer = Blit_Wrap_A_Buffer(abuffer);
                    } else {
                        abuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)abuffer);
                    }
                    zbuffer += zwidth;
                    if (zwidth > 0) {
                        zbuffer = Blit_Wrap_Z_Buffer(zbuffer);
                    } else {
                        zbuffer = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)zbuffer);
                    }
                    z += zwrap;
                }
                if (ydelta > 0) {
                    abuffer++;
                    offset += 2;
                    abuffer = Blit_Wrap_A_Buffer(abuffer);
                    zbuffer++;
                    zbuffer = Blit_Wrap_Z_Buffer(zbuffer);
                    ydelta -= dz2;
                }
                z += zstep;
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
                int v = *zbuffer;
                if (z < *abuffer && v != 0) {
                    *(unsigned short *)((char *)buffer + 2 * i) = Trail_Blend_Pixel(*(unsigned short *)((char *)buffer + 2 * i), v, inverse, red, green, blue);
                }
                if (ydelta > 0) {
                    buffer = (unsigned char *)buffer + pitch;
                    ydelta -= 2 * dx;
                    abuffer += zwidth;
                    if (zwidth > 0) {
                        abuffer = Blit_Wrap_A_Buffer(abuffer);
                    } else {
                        abuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)abuffer);
                    }
                    z += zwrap;
                    zbuffer += zwidth;
                    if (zwidth > 0) {
                        zbuffer = Blit_Wrap_Z_Buffer(zbuffer);
                    } else {
                        zbuffer = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)zbuffer);
                    }
                }
                if (zdelta > 0) {
                    z += zstep;
                    zdelta -= 2 * dx;
                }
                abuffer++;
                ydelta += dy2;
                zdelta += dz2;
                abuffer = Blit_Wrap_A_Buffer(abuffer);
                zbuffer++;
                zbuffer = Blit_Wrap_Z_Buffer(zbuffer);
            }
        }
    } else {
        int xdelta = dx2 - abs_dy;
        int zdelta = dz2 - abs_dy;
        if (abs_dy > 0) {
            int offset = 0;
            for (int i = abs_dy; i != 0; i--) {
                int v = *zbuffer;
                if (z < *abuffer && v != 0) {
                    *(unsigned short *)((char *)buffer + offset) = Trail_Blend_Pixel(*(unsigned short *)((char *)buffer + offset), v, inverse, red, green, blue);
                }
                if (xdelta > 0) {
                    abuffer++;
                    offset += 2;
                    abuffer = Blit_Wrap_A_Buffer(abuffer);
                    zbuffer++;
                    zbuffer = Blit_Wrap_Z_Buffer(zbuffer);
                    xdelta -= dy2;
                }
                if (zdelta > 0) {
                    z += zstep;
                    zdelta -= dy2;
                }
                xdelta += dx2;
                zdelta += dz2;
                buffer = (unsigned char *)buffer + pitch;
                abuffer += zwidth;
                if (zwidth > 0) {
                    abuffer = Blit_Wrap_A_Buffer(abuffer);
                } else {
                    abuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)abuffer);
                }
                z += zwrap;
                zbuffer += zwidth;
                if (zwidth > 0) {
                    zbuffer = Blit_Wrap_Z_Buffer(zbuffer);
                } else {
                    zbuffer = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)zbuffer);
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
