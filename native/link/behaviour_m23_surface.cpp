// Behaviour-written surface slots for native M23 (Scout1187), M25 (Scout1202). NOT
// EXACT; each states what it keeps.
#include "shared/blit.hpp"
#include "shared/dsurface.hpp"
#include "shared/hicolor.hpp"
#include "shared/rect.hpp"

namespace {
bool Inside(Rect const &clip, int x, int y)
{
    return x >= clip.X && x < clip.X + clip.Width && y >= clip.Y && y < clip.Y + clip.Height;
}
}

// 0x007746C0 XSurface::Draw_Ellipse (slot 8; DSurface and BSurface inherit it): the
// outline of the ellipse with radii rx, ry around center, each pixel inside the clip
// rectangle put with Put_Pixel (slot 9), as the original's midpoint walk does.
// Reached by Draw_Radial_Indicator 0x00451BF0 (the range circle of a selected or
// placed building with a radial indicator).
bool XSurface::Draw_Ellipse(Point2D center, int rx, int ry, Rect cliprect, int color)
{
    if (rx < 0 || ry < 0) {
        return false;
    }
    auto plot4 = [&](int dx, int dy) {
        int const xs[2] = {center.X + dx, center.X - dx};
        int const ys[2] = {center.Y + dy, center.Y - dy};
        for (int i = 0; i < (dx ? 2 : 1); ++i) {
            for (int j = 0; j < (dy ? 2 : 1); ++j) {
                if (Inside(cliprect, xs[i], ys[j])) {
                    Point2D p;
                    p.X = xs[i];
                    p.Y = ys[j];
                    Put_Pixel(p, color);
                }
            }
        }
    };
    long long a2 = (long long)rx * rx, b2 = (long long)ry * ry;
    int x = 0, y = ry;
    long long d = b2 - a2 * ry + a2 / 4;
    while (b2 * x <= a2 * y) {             // region 1: slope above -1
        plot4(x, y);
        if (d >= 0) {
            --y;
            d -= 2 * a2 * y;
        }
        ++x;
        d += b2 * (2 * x + 1);
    }
    d = b2 * x * x + b2 * x + b2 / 4 + a2 * (long long)(y - 1) * (y - 1) - a2 * b2;
    while (y >= 0) {                       // region 2
        plot4(x, y);
        if (d <= 0) {
            ++x;
            d += 2 * b2 * x;
        }
        --y;
        d += a2 * (1 - 2 * y);
    }
    return true;
}

// 0x004AC650 DSurface::Draw_Depth_Antialiased_Line (slot 15): the laser beam
// (LaserDrawClass::Draw_It 0x005334F0: Prism towers and laser weapons). NOT EXACT:
// behaviour written from the original's code (the pass-6a decompile is broken),
// M25 Scout1202. Kept from the original:
// - the falloff table 0x00852B70 (257 entries, built once under 0x00852FD8):
//   T[i] = ftol(((i / 128 - 1)^3 + 1) * 128), so T[a] + T[256 - a] = 256;
// - the clip rectangle: the surface rectangle shrunk by one pixel on each side,
//   intersected with cliprect; the points are relative to the clip origin, sorted
//   by x, clipped by Clip_Line, and a clipped end's depth interpolated by length;
// - a 3D Bresenham over x, y and depth with four bodies: x-major and y-major
//   (two-pixel antialiasing between the line's row/column and the next, weights
//   256 - f and f in 1/256, f stepped by minor/major * 256), and the two
//   depth-major bodies, where depth moves one unit per step (the x one also steps
//   two pixels at a time while the buffers move one: as in the original);
// - per pixel, only where the line's depth (16-bit, unsigned) is below the
//   ZBuffer value (0x00839C6C) and the ABuffer value (0x008315B4) is not zero:
//   each enabled channel becomes ((T[256-w] * back + T[w] * colour) * zvalue) >> 15
//   with w = ftol(weight * intensity), clamped to 255 (the ABuffer value is 0x7F in
//   the open and falls towards 0 under the shroud's edge, where the beam fades:
//   checked with the harness order `laser`, d/laser.png); write_depth stores the
//   line's depth into the ZBuffer. The x-major bodies blend each pixel from the
//   other one of its pair (the original reads the next row's pixel for the
//   line's pixel and the line's pixel for the next row's: kept).
// Differences: w is clamped to 0..256 (the original indexes past the table for an
// intensity above 1).
namespace {
int AA_Falloff_852b70[257];
bool AA_Falloff_Ready_852fd8;

inline int AA_Channel(int back, int color, int w, int zvalue)
{
    return ((AA_Falloff_852b70[256 - w] * back + AA_Falloff_852b70[w] * color) * zvalue) >> 15;
}

inline int AA_Weight(int weight, float intensity)
{
    int w = (int)(weight * (double)intensity);
    return w < 0 ? 0 : (w > 256 ? 256 : w);
}

// The pixel pair: first gets the blend of source1 with weight w1, second the blend
// of source2 with weight w2 (source1/source2 are first/second, or swapped in the
// x-major bodies).
void AA_Blend_Pair(unsigned short *first, unsigned short *second, bool swap, int w1, int w2, int zvalue,
                   RGBClass const &color, bool red, bool green, bool blue)
{
    RGBClass p0 = Unpack_Hicolor_Pixel(*first);
    RGBClass p1 = Unpack_Hicolor_Pixel(*second);
    RGBClass const &s1 = swap ? p1 : p0;
    RGBClass const &s2 = swap ? p0 : p1;
    int r1 = s1.Red, g1 = s1.Green, b1 = s1.Blue;
    int r2 = s2.Red, g2 = s2.Green, b2 = s2.Blue;
    if (red) {
        r1 = AA_Channel(r1, color.Red, w1, zvalue);
        r2 = AA_Channel(r2, color.Red, w2, zvalue);
    }
    if (green) {
        g1 = AA_Channel(g1, color.Green, w1, zvalue);
        g2 = AA_Channel(g2, color.Green, w2, zvalue);
    }
    if (blue) {
        b1 = AA_Channel(b1, color.Blue, w1, zvalue);
        b2 = AA_Channel(b2, color.Blue, w2, zvalue);
    }
    *first = (unsigned short)Build_Hicolor_Pixel(r1 > 255 ? 255 : r1, g1 > 255 ? 255 : g1, b1 > 255 ? 255 : b1);
    *second = (unsigned short)Build_Hicolor_Pixel(r2 > 255 ? 255 : r2, g2 > 255 ? 255 : g2, b2 > 255 ? 255 : b2);
}

template<class B>
inline void AA_Next(B *buffer, unsigned short *&pointer)
{
    pointer = buffer->Wrap_Pointer(pointer + 1);
}

template<class B>
inline void AA_Row(B *buffer, unsigned short *&pointer, int step)
{
    pointer += step;
    if (step > 0) {
        pointer = buffer->Wrap_Pointer(pointer);
    } else if ((char *)pointer < buffer->BufferHead) {
        pointer = (unsigned short *)((char *)pointer + buffer->BufferSize);
    }
}
}

bool DSurface::Draw_Depth_Antialiased_Line(Rect const &cliprect, Point2D const &startpoint, Point2D const &endpoint,
                                           RGBClass const &color, int start_depth, int end_depth, bool write_depth,
                                           bool blend_red, bool blend_green, bool blend_blue, float intensity)
{
    if (!AA_Falloff_Ready_852fd8) {
        AA_Falloff_Ready_852fd8 = true;
        for (int i = 0; i <= 256; ++i) {
            double t = i * 0.0078125 - 1.0;
            AA_Falloff_852b70[i] = (int)((t * t * t + 1.0) * 128.0);
        }
    }
    Rect surface = Get_Rect();
    Rect shrunk(surface.X + 1, surface.Y + 1, surface.Width - 2, surface.Height - 2);
    Rect clip = Intersect(cliprect, shrunk, 0, 0);
    if (clip.Width == 0 && clip.Height == 0) {
        return false;
    }
    Point2D p0(clip.X + startpoint.X, clip.Y + startpoint.Y);
    Point2D p1(clip.X + endpoint.X, clip.Y + endpoint.Y);
    int z0 = start_depth, z1 = end_depth;
    if (p0.X > p1.X) {
        Point2D t = p0; p0 = p1; p1 = t;
        int tz = z0; z0 = z1; z1 = tz;
    }
    Point2D const p0o = p0, p1o = p1;
    if (!Clip_Line(p0, p1, clip)) {
        return false;
    }
    int zs = z0, ze = z1;
    if (p1.X != p1o.X || p1.Y != p1o.Y) {
        int full = Point2D(p0o.X - p1o.X, p0o.Y - p1o.Y).Length();
        int part = Point2D(p0o.X - p1.X, p0o.Y - p1.Y).Length();
        int d = (int)((double)part / full * (double)(z0 - z1));
        d = d < 0 ? -d : d;
        ze = z0 < z1 ? z0 + d : z0 - d;
    }
    if (p0.X != p0o.X || p0.Y != p0o.Y) {
        int full = Point2D(p0o.X - p1o.X, p0o.Y - p1o.Y).Length();
        int part = Point2D(p0.X - p1o.X, p0.Y - p1o.Y).Length();
        int d = (int)((double)part / full * (double)(z0 - z1));
        d = d < 0 ? -d : d;
        zs = z1 > z0 ? z1 - d : z1 + d;
    }
    unsigned short *row = (unsigned short *)Lock(Point2D(p0.X, p0.Y));
    ZBuffer *abuf = DepthBuffer;
    ABuffer *zbuf = AlphaBuffer;
    if (abuf == 0 || zbuf == 0) {
        Unlock();
        return false;
    }
    unsigned short *aptr = abuf->Get_Buffer_Offset(Point2D(p0.X, p0.Y - abuf->Area.Y));
    unsigned short *zptr = zbuf->Get_Buffer_Offset(Point2D(p0.X - zbuf->Area.X, p0.Y - zbuf->Area.Y));
    int bufstep = abuf->Width;
    int rowdepth = -1;
    int depth = (unsigned short)(abuf->MaxValue + abuf->Area.Y) - p0.Y - clip.Y + zs;
    if (row == 0) {
        Unlock();
        return false;
    }
    int dx = p1.X - p0.X;
    int dy = p1.Y - p0.Y;
    int dz = ze - zs;
    int pitch = Stride();
    if (dy < 0) {
        pitch = -pitch;
        bufstep = -bufstep;
        rowdepth = 1;
    }
    int zsign = dz < 0 ? -1 : 1;
    int ady = dy < 0 ? -dy : dy;
    int adz = dz < 0 ? -dz : dz;
    int rowpixels = pitch / 2;
    if (dx == 0 && ady == 0) {
        Unlock();
        return false;
    }
    int w1 = 256, w2 = 0;
    auto pixel = [&](unsigned short *first, unsigned short *second, bool swap) {
        int zvalue = *zptr;
        if ((unsigned short)depth < *aptr && zvalue != 0) {
            AA_Blend_Pair(first, second, swap, AA_Weight(w1, intensity), AA_Weight(w2, intensity), zvalue, color,
                          blend_red, blend_green, blend_blue);
            if (write_depth) {
                *aptr = (unsigned short)depth;
            }
        }
    };
    if (adz > dx && adz > ady) {
        if (dx > ady) {
            // Depth-major, x minor over y.
            int step = (int)((double)ady / dx * 256.0);
            for (int i = 0; i < dx; i += 2) {
                pixel(row + i, row + i + rowpixels, true);
                w1 -= step;
                w2 += step;
                if (w2 > 256) {
                    w1 += 256;
                    w2 -= 256;
                    row = (unsigned short *)((char *)row + pitch);
                    AA_Row(abuf, aptr, bufstep);
                    AA_Row(zbuf, zptr, bufstep);
                    depth += rowdepth;
                }
                AA_Next(abuf, aptr);
                AA_Next(zbuf, zptr);
                depth += zsign;
            }
        } else {
            // Depth-major, y minor over x.
            int step = (int)((double)dx / ady * 256.0);
            int column = 0;
            for (int n = ady; n > 0; --n) {
                pixel(row + column, row + column + 1, false);
                w1 -= step;
                w2 += step;
                if (w2 > 256) {
                    w1 += 256;
                    w2 -= 256;
                    ++column;
                    AA_Next(abuf, aptr);
                    AA_Next(zbuf, zptr);
                }
                row = (unsigned short *)((char *)row + pitch);
                AA_Row(abuf, aptr, bufstep);
                AA_Row(zbuf, zptr, bufstep);
                depth += zsign + rowdepth;
            }
        }
    } else if (dx > ady) {
        // x-major.
        int step = (int)((double)ady / dx * 256.0);
        int error = 2 * adz - dx;
        for (int i = 0; i < dx; ++i) {
            pixel(row + i, row + i + rowpixels, true);
            w1 -= step;
            w2 += step;
            if (w2 > 256) {
                w1 += 256;
                w2 -= 256;
                row = (unsigned short *)((char *)row + pitch);
                AA_Row(abuf, aptr, bufstep);
                AA_Row(zbuf, zptr, bufstep);
                depth += rowdepth;
            }
            if (error > 0) {
                depth += zsign;
                error -= 2 * dx;
            }
            error += 2 * adz;
            AA_Next(abuf, aptr);
            AA_Next(zbuf, zptr);
        }
    } else {
        // y-major.
        int step = (int)((double)dx / ady * 256.0);
        int error = 2 * adz - ady;
        int column = 0;
        for (int n = ady; n > 0; --n) {
            pixel(row + column, row + column + 1, false);
            w1 -= step;
            w2 += step;
            if (w2 > 256) {
                w1 += 256;
                w2 -= 256;
                ++column;
                AA_Next(abuf, aptr);
                AA_Next(zbuf, zptr);
            }
            if (error > 0) {
                depth += zsign;
                error -= 2 * ady;
            }
            error += 2 * adz;
            row = (unsigned short *)((char *)row + pitch);
            AA_Row(abuf, aptr, bufstep);
            AA_Row(zbuf, zptr, bufstep);
            depth += rowdepth;
        }
    }
    Unlock();
    return true;
}
