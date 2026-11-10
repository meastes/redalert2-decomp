// Behaviour rows (Scout1208, native M26; NOT EXACT, native/behaviour_rewrites.json).
// WaveClass Draw_Sonic 0x007225B0 (type 0: the dolphin's sonic wave) and Draw_Laser
// 0x00722CB0 (types 1 and 2), from the pass-6a decompiles 007225b0/00722cb0 read
// against TibSun wave.cpp Draw_Sonic/Draw_Laser (the same algorithm; GPL text used as a
// shape reference, see THIRD_PARTY.md). RA2 member map: WaveStartMiddle = WaveStartMiddle,
// StartCoord.Z = StartCoord.Z, WaveShape/DrawData = Polygon_130/Scanlines_168 (rasterized
// by 0x0061BE40), Direction = Direction, LaserEC = LaserEC, the depth test reads the
// ZBuffer ring (0x00839C6C; MaxValue +0x24 is the scroll bias), LogicalSurface is the
// current drawing surface 0x0083995C, TacticalRect.Y is 0x00839634.
// Native differences (flagged): every depth value is fetched through Get_Buffer_Ptr (the
// original's slow, wrapping path; its fast path walks a running pointer when the wave fits
// before the ring's end; same values), the sonic margins are the fast path's 3 pixels, the
// laser's right/bottom limits stay inside the clip rectangle (the fast path reaches one past
// it), and a sonic pixel whose distance-table indices fall outside the 300x300 table is left
// alone (the original would read past the table).
#include "shared/wave.hpp"
#include "shared/surface.hpp"
#include "shared/blit.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/crt_memory.hpp"

void __fastcall Rasterize_Polygon_0061be40(WavePolygonInput const &shape, WaveScanlineBuffer &raster);

namespace {
inline int Min_m26(int a, int b) { return a < b ? a : b; }
inline int Max_m26(int a, int b) { return a > b ? a : b; }
}

void WaveClass::Draw_Sonic_007225b0(Point2D const &point, Rect const &cliprect)
{
    int zpix = Unknown_006A2970(StartCoord.Z);
    Surface *surface = unknown_surface_83995c;
    unsigned short *surfptr = (unsigned short *)surface->Lock(Point2D(0, 0));
    if (surfptr == 0) return;
    ZBuffer *depth = DepthBuffer;

    int top = cliprect.Y + 3;
    int bottom = cliprect.Y + cliprect.Height - 3;
    int left = cliprect.X + 3;
    int right = cliprect.X + cliprect.Width - 3;
    int xoff = point.X - WaveStartMiddle.X;
    int yoff = point.Y - WaveStartMiddle.Y;

    Rasterize_Polygon_0061be40(Polygon_130, Scanlines_168);
    if (Scanlines_168.Spans != 0 && depth != 0) {
        int starty = Scanlines_168.FirstY + unknown_int_839634 + yoff;
        int endy = Min_m26(Scanlines_168.Count + starty - 1, cliprect.Height + cliprect.Y);
        unknown_func_721cc0();
        int stride = surface->Stride() >> 1;
        bool forward = Direction > 1 && Direction < 6;
        int first = forward ? starty : endy;
        int last = forward ? endy : starty;
        int step = forward ? 1 : -1;
        unsigned short zval = forward ? (unsigned short)((unsigned short)(depth->MaxValue - zpix) - starty - 2)
                                      : (unsigned short)(depth->MaxValue - endy - zpix - 2);
        for (int y = first; forward ? y <= last : y >= last; y += step) {
            if (y >= top && y <= bottom) {
                Point2D const &span = Scanlines_168.Spans[y - starty];
                int xstart = Max_m26(left, span.X + xoff);
                int xstop = Min_m26(right, span.Y + xoff);
                int ypos = abs(y - yoff - WaveStartMiddle.Y);
                unsigned short *row = surfptr + y * stride;
                int zy = y - unknown_int_839634;
                for (int i = 0; i <= xstop - xstart; ++i) {
                    int x = forward ? xstart + i : xstop - i;
                    if (*depth->Get_Buffer_Ptr(Point2D(x, zy)) > zval) {
                        if (ypos < 300 && abs(x - WaveStartMiddle.X - xoff) < 300) {
                            Brighten_Pixel_007219c0(x, xoff, ypos, row + x);
                        }
                    }
                }
            }
            if (forward) --zval; else ++zval;
        }
    }
    surface->Unlock();
    if (Scanlines_168.Spans != 0) {
        delete [] Scanlines_168.Spans;
        Scanlines_168.Spans = 0;
    }
}

void WaveClass::Draw_Laser_00722cb0(Point2D const &point, Rect const &cliprect)
{
    if (unknown_a40b2c != 2) return;                // the highest detail level only
    int zpix = Unknown_006A2970(StartCoord.Z);
    Surface *surface = unknown_surface_83995c;
    unsigned short *buffer = (unsigned short *)surface->Lock(Point2D(0, 0));
    if (buffer == 0) return;
    ZBuffer *depth = DepthBuffer;

    int top = cliprect.Y;
    int bottom = cliprect.Y + cliprect.Height - 1;
    int left = cliprect.X;
    int right = cliprect.X + cliprect.Width - 1;
    int xoff = point.X - WaveStartMiddle.X;
    int yoff = point.Y - WaveStartMiddle.Y + unknown_int_839634;

    Rasterize_Polygon_0061be40(Polygon_130, Scanlines_168);
    if (Scanlines_168.Spans != 0 && depth != 0) {
        int ystart = yoff + Scanlines_168.FirstY;
        int yend = Min_m26(Scanlines_168.Count + ystart - 1, cliprect.Y + cliprect.Height);
        unsigned short zval = (unsigned short)((unsigned short)(depth->MaxValue - zpix) - ystart - 2);
        int stride = surface->Stride() >> 1;
        for (int y = ystart; y <= yend; ++y) {
            if (y >= top && y <= bottom) {
                Point2D const &span = Scanlines_168.Spans[y - ystart];
                int xstop = Min_m26(right, span.Y + xoff);
                int xstart = Max_m26(left, span.X + xoff);
                unsigned short *row = buffer + y * stride;
                int zy = y - unknown_int_839634;
                for (int x = xstart; x <= xstop; ++x) {
                    if (*depth->Get_Buffer_Ptr(Point2D(x, zy)) > zval) {
                        Set_Laser_Pixel_00721b00(row + x, LaserEC);
                    }
                }
            }
            --zval;
        }
    }
    surface->Unlock();
    if (Scanlines_168.Spans != 0) {
        delete [] Scanlines_168.Spans;
        Scanlines_168.Spans = 0;
    }
}
