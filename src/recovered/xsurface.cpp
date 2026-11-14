// xsurface.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/xsurface.json carries the addresses and the rows that
// stay in their own files.


#define RA2_RECT_POINT_INLINE
#include "shared/unknown_static_rect.hpp"
#include "shared/surface.hpp"
#include "shared/crt_memory.hpp"
#include "shared/blitter.hpp"
#include "shared/rect.hpp"


// Header-static dynamic initializer of the unit at 0x00773930 (xsurface.cpp):
// the file-local all-zero Rect of shared/unknown_static_rect.hpp
// (0x00B2AF70). The other drafts of this plan follow.


// bool XSurface::Draw_Line(Point2D const &startpoint, Point2D const &endpoint, int color) (0x00773950, slot 12)
// Draws a line clipped to the whole surface.

bool XSurface::Draw_Line(Point2D const &startpoint, Point2D const &endpoint, int color)
{
    return XSurface::Draw_Line(Get_Rect(), startpoint, endpoint, color);
}


// Clip a line, lock at its first endpoint, and write its pixels in the surface format.
bool XSurface::Draw_Line(Rect const &cliprect, Point2D const &startpoint, Point2D const &endpoint, int color)
{
    Rect clip = cliprect.Intersect(Get_Rect());
    Point2D start = Offset_Point(startpoint, clip);
    Point2D end = Offset_Point(endpoint, clip);
    if (Clip_Line(start, end, clip)) {
        if (start.X > end.X) {
            Point2D temp = start;
            start = end;
            end = temp;
        }
        int bytes_per_pixel = Bytes_Per_Pixel();
        unsigned char *pixels = (unsigned char *)Lock(start);
        if (pixels) {
            unsigned char color8 = (unsigned char)color;
            unsigned short color16 = (unsigned short)color;
            if (start.Y == end.Y) {
                if (bytes_per_pixel == 1) {
                    memset(pixels, color, end.X - start.X + 1);
                } else {
                    for (int index = 0; index <= end.X - start.X; pixels += 2) {
                        *(unsigned short *)pixels = color16;
                        ++index;
                    }
                }
            } else if (start.X == end.X) {
                int stride;
                if (start.Y > end.Y) stride = -Stride();
                else stride = Stride();
                int height = abs(end.Y - start.Y);
                for (int index = 0; index <= height; ++index) {
                    if (bytes_per_pixel == 1) *pixels = color8;
                    else *(unsigned short *)pixels = color16;
                    pixels += stride;
                }
            } else {
                int dx = end.X - start.X;
                int dy = end.Y - start.Y;
                int stride = Stride();
                if (dy < 0) stride = -stride;
                dy = abs(dy);
                int dx2 = dx * 2;
                int dy2 = dy * 2;
                if (dx > dy) {
                    int error = dy2 - dx;
                    for (int index = 0; index < dx; ++index) {
                        if (bytes_per_pixel == 1) pixels[index] = color8;
                        else ((unsigned short *)pixels)[index] = color16;
                        if (error > 0) {
                            pixels += stride;
                            error -= dx2;
                        }
                        error += dy2;
                    }
                } else {
                    int error = dx2 - dy;
                    int offset = 0;
                    for (int count = dy; count > 0; --count) {
                        if (bytes_per_pixel == 1) pixels[offset] = color8;
                        else ((unsigned short *)pixels)[offset] = color16;
                        if (error > 0) {
                            ++offset;
                            error -= dy2;
                        }
                        error += dx2;
                        pixels += stride;
                    }
                }
            }
            Unlock();
            return true;
        }
    }
    return false;
}


// Draw the 16-entry dash pattern along a line and return its updated phase.
int XSurface::Draw_Dashed_Line(Point2D const &startpoint, Point2D const &endpoint, int color,
bool const *pattern, int phase)
{
    Point2D start = startpoint;
    Point2D end = endpoint;
    int step = 1;
    if (start.X > end.X) {
        Point2D temp = start;
        start = end;
        end = temp;
        int length = abs(start.X - end.X);
        int height = abs(start.Y - end.Y) + 1;
        if (length < height) length = height;
        phase = (phase + length) % 16;
        step = -1;
    }
    int bytes_per_pixel = Bytes_Per_Pixel();
    unsigned char *pixels = (unsigned char *)Lock(start);
    if (pixels) {
        unsigned char color8 = (unsigned char)color;
        unsigned short color16 = (unsigned short)color;
        if (start.Y == end.Y) {
            // The original horizontal path writes 16-bit pixels unconditionally.
            int width = end.X - start.X;
            for (int index = 0; index <= width; ++index) {
                if (phase < 0) phase += 16;
                if (phase >= 16) phase -= 16;
                if (pattern[phase]) *(unsigned short *)pixels = color16;
                pixels += 2;
                phase += step;
            }
        } else if (start.X == end.X) {
            int stride;
            if (start.Y > end.Y) stride = -Stride();
            else stride = Stride();
            int height = abs(end.Y - start.Y);
            for (int index = 0; index <= height; ++index) {
                if (phase < 0) phase += 16;
                if (phase >= 16) phase -= 16;
                if (pattern[phase]) {
                    if (bytes_per_pixel == 1) *pixels = color8;
                    else *(unsigned short *)pixels = color16;
                }
                pixels += stride;
                phase += step;
            }
        } else {
            int dx = end.X - start.X;
            int dy = end.Y - start.Y;
            int stride = Stride();
            if (dy < 0) stride = -stride;
            dy = abs(dy);
            int dx2 = dx * 2;
            int dy2 = dy * 2;
            if (dx > dy) {
                int error = dy2 - dx;
                for (int index = 0; index < dx; phase += step) {
                    if (phase < 0) phase += 16;
                    if (phase >= 16) phase -= 16;
                    if (pattern[phase]) {
                        if (bytes_per_pixel == 1) pixels[index] = color8;
                        else ((unsigned short *)pixels)[index] = color16;
                    }
                    if (error > 0) {
                        pixels += stride;
                        error -= dx2;
                    }
                    error += dy2;
                    ++index;
                }
            } else {
                int error = dx2 - dy;
                int offset = 0;
                for (int count = dy; count > 0; --count) {
                    if (phase < 0) phase += 16;
                    if (phase >= 16) phase -= 16;
                    if (pattern[phase]) {
                        if (bytes_per_pixel == 1) pixels[offset] = color8;
                        else ((unsigned short *)pixels)[offset] = color16;
                    }
                    if (error > 0) {
                        ++offset;
                        error -= dy2;
                    }
                    error += dx2;
                    pixels += stride;
                    phase += step;
                }
            }
        }
        Unlock();
    }
    return phase;
}


// int XSurface::Draw_Dashed_Line_Depth_Masked(...) (0x00773EA0, slot 19)
// The generic surface has no depth buffer: draws nothing and returns 0
// (DSurface 0x004B0650 implements it for 16-bit surfaces).

int XSurface::Draw_Dashed_Line_Depth_Masked(Point2D const &, Point2D const &, int, bool const *, int, bool)
{
    return 0;
}


// bool XSurface::Draw_Line_Depth_Masked(...) (0x00773EB0, slot 20)
// The generic surface has no depth buffer: draws nothing and returns false
// (DSurface 0x004B0D30 implements it for 16-bit surfaces).

bool XSurface::Draw_Line_Depth_Masked(Point2D const &, Point2D const &, int, bool)
{
    return false;
}


// bool XSurface::Draw_Depth_Shaded_Line(Rect const &, Point2D const &, Point2D const &, unsigned,
// int, int, bool) (0x00773EC0, slot 13)
// Not implemented by the generic surface: returns false (DSurface overrides it, 0x004AFC30).
// Name and parameter list from TibSun's XSurface (7 stack arguments, ret 0x1c).

bool XSurface::Draw_Depth_Shaded_Line(Rect const &, Point2D const &, Point2D const &, unsigned, int, int, bool)
{
    return false;
}


// bool XSurface::Draw_Depth_Glow_Line(Rect const &, Point2D const &, Point2D const &,
//     int, int, int, bool) (0x00773ED0, slot 14)
// Not implemented by the generic surface: returns false. The signature is
// TibSun's (dsurface.h Draw_Depth_Glow_Line; 7 stack arguments, ret 0x1c);
// DSurface overrides it (0x004ABBA0).

bool XSurface::Draw_Depth_Glow_Line(Rect const &, Point2D const &, Point2D const &, int, int, int, bool)
{
    return false;
}


// bool XSurface::Draw_Depth_Antialiased_Line(Rect const &, Point2D const &, Point2D const &,
// RGBClass const &, int, int, bool, bool, bool, bool, float) (0x00773EE0, slot 15)
// Not implemented by the generic surface: returns false (DSurface overrides it, 0x004AC650).
// Name and parameter list from TibSun's XSurface (11 stack arguments, ret 0x2c).

bool XSurface::Draw_Depth_Antialiased_Line(Rect const &, Point2D const &, Point2D const &, RGBClass const &, int,
                                           int, bool, bool, bool, bool, float)
{
    return false;
}


// bool XSurface::Unknown_Slot16(Rect const &, Point2D const &, Point2D const &, RGBClass const &, float,
// int, int) (0x00773EF0, slot 16)
// Not implemented by the generic surface: returns false. DSurface's 0x004ADE00 reads the colour's
// three bytes and scales them by the float; LaserDrawClass::Draw_It 0x005334F0 passes the depths last.

bool XSurface::Unknown_Slot16(Rect const &, Point2D const &, Point2D const &, RGBClass const &, float, int, int)
{
    return false;
}


// Not exact (RA2_NON_MATCHING): XSurface::Plot_Line at 0x00773F00, best score 99.44, 2026-11-13; differs: same instructions; registers, stack slots or operands differ
#ifdef RA2_NON_MATCHING
// bool XSurface::Plot_Line (0x00773F00, slot 17)
bool XSurface::Plot_Line(Rect const &cliprect, Point2D const &startpoint, Point2D const &endpoint,
                         void (__fastcall *plotter)(Point2D const &))
{
    Rect clip = cliprect.Intersect(Get_Rect());
    Point2D start = Offset_Point(startpoint, clip);
    Point2D end = Offset_Point(endpoint, clip);
    if (!Clip_Line(start, end, clip)) return false;
    if (start.X > end.X) {
        Point2D temp = start;
        start = end;
        end = temp;
    }
    if (start.Y == end.Y) {
        int width = end.X - start.X;
        for (int index = 0; index <= width; ++index) {
            plotter(Point2D(start.X + index, end.Y));
        }
    } else if (start.X == end.X) {
        int height = abs(start.Y - end.Y);
        int top = end.Y;
        if (start.Y <= end.Y) top = start.Y;
        for (int index = 0; index <= height; ++index) {
            plotter(Point2D(start.X, top + index));
        }
    } else {
        int x = start.X;
        int step = 1;
        int y = start.Y;
        int dx = end.X - x;
        int dy = end.Y - y;
        if (dy < 0) step = -1;
        dy = abs(dy);
        int dx2 = dx * 2;
        int dy2 = dy * 2;
        if (dx > dy) {
            int error = dy2 - dx;
            int x2 = end.X;
            int y2 = y;
            for (int cursor = x; cursor <= x2; ++cursor) {
                plotter(Point2D(cursor, y2));
                if (error > 0) {
                    error -= dx2;
                    y2 += step;
                }
                error += dy2;
            }
        } else {
            int error = dx2 - dy;
            for (int index = 0; index <= dy; ++index) {
                plotter(Point2D(x, y));
                if (error > 0) {
                    ++x;
                    error -= dy2;
                }
                error += dx2;
                y += step;
            }
        }
    }
    Unlock();
    return true;
}
#endif

// bool XSurface::Draw_Rect(Rect const &rect, int color) (0x00774100, slot 22)
// Outlines rect clipped to the whole surface.

bool XSurface::Draw_Rect(Rect const &rect, int color)
{
    return XSurface::Draw_Rect(Get_Rect(), rect, color);
}


// bool XSurface::Draw_Rect(Rect const &cliprect, Rect const &rect, int color) (0x00774130, slot 21)
// Outlines rect (inclusive corners) with four clipped Draw_Line calls.

bool XSurface::Draw_Rect(Rect const &cliprect, Rect const &rect, int color)
{
    Point2D topleft;
    Point2D topright;
    Point2D bottomright;
    Point2D bottomleft;
    topleft.X = rect.X;
    topleft.Y = rect.Y;
    topright.X = rect.X + rect.Width - 1;
    topright.Y = rect.Y;
    bottomleft.X = rect.X;
    bottomleft.Y = rect.Y + rect.Height - 1;
    bottomright.X = rect.X + rect.Width - 1;
    bottomright.Y = rect.Y + rect.Height - 1;

    Draw_Line(cliprect, topleft, topright, color);
    Draw_Line(cliprect, topleft, bottomleft, color);
    Draw_Line(cliprect, topright, bottomright, color);
    Draw_Line(cliprect, bottomleft, bottomright, color);
    return true;
}


// int XSurface::Get_Pixel(Point2D const &point) (0x007741D0, slot 10)
// Reads one pixel (1 or 2 bytes); 0 when the surface cannot be locked.

int XSurface::Get_Pixel(Point2D const &point)
{
    int color = 0;
    void *pixel = Lock(point);
    if (pixel != 0) {
        if (Bytes_Per_Pixel() == 2) {
            color = *(unsigned short *)pixel;
        } else {
            color = *(unsigned char *)pixel;
        }
        Unlock();
    }
    return color;
}


// bool XSurface::Put_Pixel(Point2D const &point, int color) (0x00774220, slot 9)
// Stores one pixel (1 or 2 bytes); false when the surface cannot be locked.

bool XSurface::Put_Pixel(Point2D const &point, int color)
{
    void *pixel = Lock(point);
    if (pixel != 0) {
        if (Bytes_Per_Pixel() == 2) {
            *(unsigned short *)pixel = (unsigned short)color;
        } else {
            *(unsigned char *)pixel = (unsigned char)color;
        }
        Unlock();
        return true;
    }
    return false;
}


// int XSurface::Get_Pixel(Point2D const &point, Rect const &cliprect) (0x00774280, slot 35)
// Reads one pixel when the point lies inside cliprect; 0 otherwise.
// The point goes to Lock by value (Lock takes a Point2D).

int XSurface::Get_Pixel(Point2D const &point, Rect const &cliprect)
{
    int color = 0;
    if (cliprect.Is_Point_Within(point)) {
        void *pixel = Lock(point);
        if (pixel != 0) {
            if (Bytes_Per_Pixel() == 2) {
                color = *(unsigned short *)pixel;
            } else {
                color = *(unsigned char *)pixel;
            }
            Unlock();
        }
    }
    return color;
}


// bool XSurface::Put_Pixel(Point2D const &point, int color, Rect const &cliprect) (0x00774300, slot 34)
// Stores one pixel when the point lies inside cliprect.
// The point goes to Lock by value (Lock takes a Point2D).

bool XSurface::Put_Pixel(Point2D const &point, int color, Rect const &cliprect)
{
    if (cliprect.Is_Point_Within(point)) {
        void *pixel = Lock(point);
        if (pixel != 0) {
            if (Bytes_Per_Pixel() == 2) {
                *(unsigned short *)pixel = (unsigned short)color;
            } else {
                *(unsigned char *)pixel = (unsigned char)color;
            }
            Unlock();
            return true;
        }
    }
    return false;
}


// bool XSurface::Fill_Rect(Rect const &fillrect, int color) (0x00774390, slot 5)
// Fills fillrect: passes it as the clip rectangle and the whole surface
// rectangle (placed relative to it) as the area, so exactly fillrect,
// clipped to the surface, is filled.

bool XSurface::Fill_Rect(Rect const &fillrect, int color)
{
    return XSurface::Fill_Rect(fillrect, Get_Rect(), color);
}


// bool XSurface::Fill_Rect_Trans(Rect const &rect, RGBClass const &color, unsigned int percent)
// (0x007746B0, slot 7)
// Not implemented by the generic surface: returns false (DSurface blends,
// 0x004AB730).

bool XSurface::Fill_Rect_Trans(Rect const &, RGBClass const &, unsigned int)
{
    return false;
}


// Not exact (RA2_NON_MATCHING): XSurface::Draw_Ellipse at 0x007746C0, best score 22.29, 2026-11-13; differs: 483 instructions in the original, 477 in the draft (shape 79.79)
#ifdef RA2_NON_MATCHING
bool XSurface::Draw_Ellipse(Point2D point, int radx, int rady, Rect cliprect, int color)
{
    Point2D pixel;
    int pitch = Stride() >> 1;

    int bottom = point.Y + rady;
    if (point.X >= cliprect.X && point.X < cliprect.X + cliprect.Width && bottom >= cliprect.Y && bottom < cliprect.Y + cliprect.Height) {
        pixel.X = point.X;
        pixel.Y = bottom;
        Put_Pixel(pixel, color);
    }

    int top = point.Y - rady;
    if (point.X >= cliprect.X && point.X < cliprect.X + cliprect.Width && top >= cliprect.Y && top < cliprect.Y + cliprect.Height) {
        pixel.X = point.X;
        pixel.Y = top;
        Put_Pixel(pixel, color);
    }

    int rady_squared = rady * rady;
    int radx_squared = radx * radx;
    int product = rady * radx_squared;
    int y = rady;
    int xterm = 0;
    int yterm = 2 * product;
    int decision = radx_squared / 4 - product;

    short *buffer = (short *)Lock();

    int bottom_offset = pitch * bottom;
    int top_offset = pitch * top;
    int xleft = point.X;
    short *bottom_right = &buffer[bottom_offset + point.X];
    short *top_left = &buffer[top_offset + point.X];
    short *top_right = &buffer[top_offset + point.X];
    int xright = point.X;
    short *bottom_left = &buffer[bottom_offset + point.X];

    while (true) {
        decision += rady_squared + xterm;
        if (decision >= 0) {
            yterm -= 2 * radx_squared;
            decision -= yterm;
            y--;

            bottom_right -= pitch;
            top_left += pitch;
            top_right += pitch;
            bottom_left -= pitch;
        }
        xterm += 2 * rady_squared;

        xright++;
        xleft--;

        bottom_right++;
        top_left--;
        top_right++;
        bottom_left--;

        if (xterm >= yterm) {
            break;
        }
        int ypos = y + point.Y;
        if (xright >= cliprect.X && xright < cliprect.X + cliprect.Width && ypos >= cliprect.Y && ypos < cliprect.Y + cliprect.Height) {
            *bottom_right = color;
        }
        ypos = point.Y - y;
        if (xleft >= cliprect.X && xleft < cliprect.X + cliprect.Width && ypos >= cliprect.Y && ypos < cliprect.Y + cliprect.Height) {
            *top_left = color;
        }
        ypos = point.Y - y;
        if (xright >= cliprect.X && xright < cliprect.X + cliprect.Width && ypos >= cliprect.Y && ypos < cliprect.Y + cliprect.Height) {
            *top_right = color;
        }
        ypos = point.Y + y;
        if (xleft >= cliprect.X && xleft < cliprect.X + cliprect.Width && ypos >= cliprect.Y && ypos < cliprect.Y + cliprect.Height) {
            *bottom_left = color;
        }
    }

    xright = point.X + radx;
    if (xright >= cliprect.X && xright < cliprect.X + cliprect.Width && point.Y >= cliprect.Y && point.Y < cliprect.Y + cliprect.Height) {
        pixel.X = xright;
        pixel.Y = point.Y;
        Put_Pixel(pixel, color);
    }

    xleft = point.X - radx;
    if (xleft >= cliprect.X && xleft < cliprect.X + cliprect.Width && point.Y >= cliprect.Y && point.Y < cliprect.Y + cliprect.Height) {
        pixel.X = xleft;
        pixel.Y = point.Y;
        Put_Pixel(pixel, color);
    }

    y = 0;
    yterm = 0;
    product = radx * rady_squared;
    xterm = 2 * product;
    int x = radx;
    decision = rady_squared / 4 - product;

    int left_offset = pitch * point.Y - radx;
    int center_offset = pitch * point.Y;
    xleft = point.X - radx;
    bottom_right = &buffer[point.X + center_offset + radx];
    top_left = &buffer[xleft + center_offset];
    xright = point.X + radx;
    top_right = &buffer[xright + center_offset];
    bottom_left = &buffer[point.X + left_offset];

    while (true) {
        decision += radx_squared + yterm;
        if (decision >= 0) {
            xterm -= 2 * rady_squared;
            decision -= xterm;
            bottom_right--;
            xright--;
            top_left++;
            xleft++;
            top_right--;
            bottom_left++;
        }
        yterm += 2 * radx_squared;
        y++;

        bottom_right += pitch;
        top_left -= pitch;
        top_right -= pitch;
        bottom_left += pitch;

        if (yterm > xterm) {
            break;
        }
        int ypos = y + point.Y;
        if (xright >= cliprect.X && xright < cliprect.X + cliprect.Width && ypos >= cliprect.Y && ypos < cliprect.Y + cliprect.Height) {
            *bottom_right = color;
        }
        ypos = point.Y - y;
        if (xleft >= cliprect.X && xleft < cliprect.X + cliprect.Width && ypos >= cliprect.Y && ypos < cliprect.Y + cliprect.Height) {
            *top_left = color;
        }
        ypos = point.Y - y;
        if (xright >= cliprect.X && xright < cliprect.X + cliprect.Width && ypos >= cliprect.Y && ypos < cliprect.Y + cliprect.Height) {
            *top_right = color;
        }
        ypos = point.Y + y;
        if (xleft >= cliprect.X && xleft < cliprect.X + cliprect.Width && ypos >= cliprect.Y && ypos < cliprect.Y + cliprect.Height) {
            *bottom_left = color;
        }
    }

    return Unlock();
}
#endif

// void XSurface::Fill_Circle (0x00774C90; TibSun xsurface.cpp Draw_Circle): fills the
// circle with horizontal clipped lines, stepping the midpoint circle algorithm
// through one octant and mirroring it.

void XSurface::Fill_Circle(Point2D center, int radius, Rect cliprect, int color)
{
    Point2D pt(radius, 0);
    const int roundness_val = 2;
    int d = 3 - (roundness_val * radius);
    do {
        Draw_Line(cliprect, center + Point2D(-pt.X, pt.Y), center + Point2D(pt.X, pt.Y), color);
        Draw_Line(cliprect, center + Point2D(-pt.Y, pt.X), center + Point2D(pt.Y, pt.X), color);
        Draw_Line(cliprect, center + Point2D(-pt.X, -pt.Y), center + Point2D(pt.X, -pt.Y), color);
        Draw_Line(cliprect, center + Point2D(-pt.Y, -pt.X), center + Point2D(pt.Y, -pt.X), color);
        if (d < 0) {
            d += (4 * pt.Y) + 6;
        } else {
            d += 4 * (pt.Y - pt.X) + 10;
            pt.X--;
        }
        pt.Y++;
    } while (pt.X >= pt.Y);
}


// bool XSurface::Fill(int color) (0x00774E20, slot 6)
// Fills the whole surface.

bool XSurface::Fill(int color)
{
    return Fill_Rect(Get_Rect(), Get_Rect(), color);
}


// bool XSurface::Copy_From(Surface &source, bool trans, bool unknown) (0x00774E60, slot 1)
// Copies the whole source surface to the top left corner.

bool XSurface::Copy_From(Surface &source, bool trans, bool unknown)
{
    Rect torect = Get_Rect();
    torect.X = 0;
    torect.Y = 0;
    Rect fromrect = source.Get_Rect();
    fromrect.X = 0;
    fromrect.Y = 0;
    return Copy_From(torect, source, fromrect, trans, unknown);
}


// bool XSurface::Copy_From(Rect const &toarea, Surface &source, Rect const &fromarea, bool trans, bool unknown) (0x00774F00, slot 2)
// Copies fromarea of source to toarea, both clipped to their surfaces: the
// seven-argument copy (slot 3) with the two surfaces' rectangles as windows.
// VC6 expands the seven-argument copy (0x00775060, defined later in the
// unit) and its blitter helpers inline here.

bool XSurface::Copy_From(Rect const &toarea, Surface &source, Rect const &fromarea, bool trans, bool unknown)
{
    return XSurface::Copy_From(Get_Rect(), toarea, source, source.Get_Rect(), fromarea, trans, unknown);
}


// bool XSurface::Copy_From(Rect const &toclip, Rect const &toarea, Surface &source, Rect const &fromclip, Rect const &fromarea, bool trans, bool unknown) (0x00775060, slot 3)
// Copies fromarea of source to toarea, clipped to fromclip and toclip, with
// the plain or transparent blitter for the surface's pixel size.

bool XSurface::Copy_From(Rect const &toclip, Rect const &toarea, Surface &source, Rect const &fromclip,
                         Rect const &fromarea, bool trans, bool unknown)
{
    Rect torect = toarea;
    Rect fromrect = fromarea;
    if (Clip_Blit_Rects(torect, toclip, fromrect, fromclip)) {
        if (trans) {
            return Bit_Blit_Trans(*this, torect, source, fromrect);
        }
        return Bit_Blit_Plain(*this, torect, source, fromrect);
    }
    return false;
}


// bool __fastcall Clip_Blit_Rects(Rect &toarea, Rect const &towindow, Rect &fromarea, Rect const &fromwindow) (0x00775190)
// Clips a destination and a source area against their windows. Equal-sized
// areas are trimmed together so they stay the same size; otherwise each is
// just intersected with its window. False when nothing is left.

bool __fastcall Clip_Blit_Rects(Rect &toarea, Rect const &towindow, Rect &fromarea, Rect const &fromwindow)
{
    if (toarea.Width == fromarea.Width && toarea.Height == fromarea.Height) {
        if (toarea.X < 0) {
            fromarea.X -= toarea.X;
            fromarea.Width += toarea.X;
            toarea.Width += toarea.X;
            toarea.X = 0;
        }
        if (toarea.Y < 0) {
            fromarea.Y -= toarea.Y;
            fromarea.Height += toarea.Y;
            toarea.Height += toarea.Y;
            toarea.Y = 0;
        }
        int overlap = toarea.X - towindow.Width + toarea.Width;
        if (overlap > 0) {
            fromarea.Width -= overlap;
            toarea.Width -= overlap;
        }
        overlap = toarea.Height - towindow.Height + toarea.Y;
        if (overlap > 0) {
            fromarea.Height -= overlap;
            toarea.Height -= overlap;
        }

        if (fromarea.X < 0) {
            toarea.X -= fromarea.X;
            fromarea.Width += fromarea.X;
            toarea.Width += fromarea.X;
            fromarea.X = 0;
        }
        if (fromarea.Y < 0) {
            toarea.Y -= fromarea.Y;
            fromarea.Height += fromarea.Y;
            toarea.Height += fromarea.Y;
            fromarea.Y = 0;
        }
        overlap = fromarea.Width - fromwindow.Width + fromarea.X;
        if (overlap > 0) {
            fromarea.Width -= overlap;
            toarea.Width -= overlap;
        }
        overlap = fromarea.Y - fromwindow.Height + fromarea.Height;
        if (overlap > 0) {
            fromarea.Height -= overlap;
            toarea.Height -= overlap;
        }
    } else {
        toarea = toarea.Intersect(towindow);
        fromarea = fromarea.Intersect(fromwindow);
    }
    return toarea.Is_Valid() && fromarea.Is_Valid();
}


// bool __fastcall Prep_For_Blit(Surface &dest, Rect &drect, Surface &source, Rect &srect, bool &overlapped, void *&dbuffer, void *&sbuffer) (0x00775360)
// Prepares a blit clipped to the two surfaces' own rectangles.

bool __fastcall Prep_For_Blit(Surface &dest, Rect &drect, Surface &source, Rect &srect, bool &overlapped,
                              void *&dbuffer, void *&sbuffer)
{
    return Prep_For_Blit(dest, dest.Get_Rect(), drect, source, source.Get_Rect(), srect, overlapped, dbuffer,
                         sbuffer);
}


// bool Prep_For_Blit(Surface &dest, Rect const &dcliprect, Rect &drect, Surface &source,
//                    Rect const &scliprect, Rect &srect, bool &overlapped, void *&dbuffer,
//                    void *&sbuffer) (0x007753B0)
// Clips both areas against their windows, marks a copy within one surface
// whose source starts above (or level and left of) its overlapping
// destination as overlapped (it must run backward), and locks both surfaces
// at the areas' top left corners; the destination is unlocked again when the
// source cannot be locked.

bool __fastcall Prep_For_Blit(Surface &dest, Rect const &dcliprect, Rect &drect, Surface &source,
                              Rect const &scliprect, Rect &srect, bool &overlapped, void *&dbuffer,
                              void *&sbuffer)
{
    overlapped = false;
    dbuffer = 0;
    sbuffer = 0;
    if (drect.Is_Valid() && dcliprect.Is_Valid() && srect.Is_Valid() && scliprect.Is_Valid()
        && Clip_Blit_Rects(drect, dcliprect, srect, scliprect)) {
        overlapped = false;
        if (&source == &dest) {
            if (srect.X < drect.X + drect.Width && srect.Y < drect.Y + drect.Height
                && srect.X + srect.Width > drect.X && srect.Y + srect.Height > drect.Y
                && (srect.Y < drect.Y || (srect.Y == drect.Y && srect.X < drect.X))) {
                overlapped = true;
            }
        }
        dbuffer = dest.Lock(dcliprect.Top_Left() + drect.Top_Left());
        if (dbuffer) {
            sbuffer = source.Lock(scliprect.Top_Left() + srect.Top_Left());
            if (!sbuffer) {
                dest.Unlock();
            } else {
                return true;
            }
        }
    }
    return false;
}


// bool Bit_Blit_Plain(Surface &dest, Rect const &destrect, Surface &source, Rect const &sourcerect) (0x00775560)
// Copies sourcerect of source to destrect of dest with the plain blitter for
// dest's pixel size. The name is a hypothesis (Westwood's Bit_Blit overload
// without the transparency flag); the calling convention is __fastcall like
// Bit_Blit itself.

bool __fastcall Bit_Blit_Plain(Surface &dest, Rect const &destrect, Surface &source, Rect const &sourcerect)
{
    if (dest.Bytes_Per_Pixel() == 1) {
        return Bit_Blit(dest, destrect, source, sourcerect, BlitPlain<unsigned char>(), 0, 3, 1000, 0);
    }
    return Bit_Blit(dest, destrect, source, sourcerect, BlitPlain<unsigned short>(), 0, 3, 1000, 0);
}


// bool Bit_Blit_Trans(Surface &dest, Rect const &destrect, Surface &source, Rect const &sourcerect) (0x007755C0)
// Copies sourcerect of source to destrect of dest with the transparent blitter (colour 0 is skipped) for
// dest's pixel size. The name is a hypothesis (Westwood's Bit_Blit overload
// with the transparency flag set); the calling convention is __fastcall like
// Bit_Blit itself.

bool __fastcall Bit_Blit_Trans(Surface &dest, Rect const &destrect, Surface &source, Rect const &sourcerect)
{
    if (dest.Bytes_Per_Pixel() == 1) {
        return Bit_Blit(dest, destrect, source, sourcerect, BlitTrans<unsigned char>(), 0, 3, 1000, 0);
    }
    return Bit_Blit(dest, destrect, source, sourcerect, BlitTrans<unsigned short>(), 0, 3, 1000, 0);
}


// bool Clip_Line(Point2D &start, Point2D &end, Rect const &rect) (0x00775620)
// Cohen-Sutherland clipping of the line start-end to rect, in doubles: while
// an endpoint lies outside (Clip_Line_Outcode, expanded inline), move it to
// the edge it crosses (the last row or column inside for the excluded right
// and bottom edges). False when both endpoints lie beyond one edge; true with
// the endpoints rewritten otherwise.

bool __fastcall Clip_Line(Point2D &start, Point2D &end, Rect const &rect)
{
    double x = rect.X;
    double y = rect.Y;
    double x0 = start.X;
    double y0 = start.Y;
    double x1 = end.X;
    double y1 = end.Y;
    double inverse_slope = (x1 - x0) / (y1 - y0);
    double slope = (y1 - y0) / (x1 - x0);
    int code0 = Clip_Line_Outcode(rect, x0, y0);
    int code1 = Clip_Line_Outcode(rect, x1, y1);
    for (;;) {
        if (code0 == 0 && code1 == 0) {
            start.X = (int)x0;
            start.Y = (int)y0;
            end.X = (int)x1;
            end.Y = (int)y1;
            return true;
        }
        if (code0 & code1) {
            return false;
        }
        int code = code0 ? code0 : code1;
        if (code & 8) {
            x = x0 + (rect.Y - y0) * inverse_slope;
            y = rect.Y;
        } else if (code & 4) {
            x = x0 + (rect.Y + rect.Height - 1 - y0) * inverse_slope;
            y = rect.Y + rect.Height - 1;
        } else if (code & 2) {
            y = y0 + (rect.X + rect.Width - 1 - x0) * slope;
            x = rect.X + rect.Width - 1;
        } else if (code & 1) {
            y = y0 + (rect.X - x0) * slope;
            x = rect.X;
        }
        if (code == code0) {
            x0 = x;
            y0 = y;
            code0 = Clip_Line_Outcode(rect, x0, y0);
        } else {
            x1 = x;
            y1 = y;
            code1 = Clip_Line_Outcode(rect, x1, y1);
        }
    }
}


// int Clip_Line_Outcode(Rect const &rect, double x, double y) (0x007758F0)
// Cohen-Sutherland region code of a point against rect: 1 left, 2 right
// (at or beyond the excluded right edge), 8 above, 4 at or below the
// excluded bottom edge. Clip_Line (0x00775620) expands it inline. The name is
// a hypothesis.

int __fastcall Clip_Line_Outcode(Rect const &rect, double x, double y)
{
    int code = 0;
    if (x >= rect.X + rect.Width) {
        code = 2;
    } else if (x < rect.X) {
        code = 1;
    }
    if (y >= rect.Y + rect.Height) {
        code |= 4;
    } else if (y < rect.Y) {
        code |= 8;
    }
    return code;
}


// BlitPlain<unsigned char> scalar deleting destructor (0x007759B0)
// Compiler generated with the BlitPlain<unsigned char> vtable.
// Template member defined in shared/blitter.hpp; instantiated in this unit
// by the blitter temporaries of XSurface::Copy_From.


// BlitPlain<unsigned short> scalar deleting destructor (0x007759D0)
// Compiler generated with the BlitPlain<unsigned short> vtable.
// Template member defined in shared/blitter.hpp; instantiated in this unit
// by the blitter temporaries of XSurface::Copy_From.


// BlitTrans<unsigned char> scalar deleting destructor (0x007759F0)
// Compiler generated with the BlitTrans<unsigned char> vtable.
// Template member defined in shared/blitter.hpp; instantiated in this unit
// by the blitter temporaries of XSurface::Copy_From.


// BlitTrans<unsigned short> scalar deleting destructor (0x00775A10)
// Compiler generated with the BlitTrans<unsigned short> vtable.
// Template member defined in shared/blitter.hpp; instantiated in this unit
// by the blitter temporaries of XSurface::Copy_From.


// void BlitTrans<unsigned char>::BlitBackward(...) const (0x00775A30)
// Forwards to BlitForward with a zero last argument.
// Template member defined in shared/blitter.hpp; instantiated in this unit
// by the blitter temporaries of XSurface::Copy_From.


// void BlitPlain<unsigned char>::BlitForward(...) const (0x00775A60)
// memcpy of length bytes.
// Template member defined in shared/blitter.hpp; instantiated in this unit
// by the blitter temporaries of XSurface::Copy_From.


// void BlitPlain<unsigned char>::BlitBackward(...) const (0x00775A90)
// memmove of length bytes (overlap safe).
// Template member defined in shared/blitter.hpp; instantiated in this unit
// by the blitter temporaries of XSurface::Copy_From.


// void BlitPlain<unsigned short>::BlitForward(...) const (0x00775AB0)
// memcpy of length 16-bit pixels.
// Template member defined in shared/blitter.hpp; instantiated in this unit
// by the blitter temporaries of XSurface::Copy_From.


// void BlitPlain<unsigned short>::BlitBackward(...) const (0x00775AE0)
// memmove of length 16-bit pixels.
// Template member defined in shared/blitter.hpp; instantiated in this unit
// by the blitter temporaries of XSurface::Copy_From.


// void BlitTrans<unsigned short>::BlitForward(...) const (0x00775B00)
// Copies the non-zero 16-bit pixels.
// Template member defined in shared/blitter.hpp; instantiated in this unit
// by the blitter temporaries of XSurface::Copy_From.


// void BlitTrans<unsigned short>::BlitBackward(...) const (0x00775B30)
// Forwards to BlitForward with a zero last argument.
// Template member defined in shared/blitter.hpp; instantiated in this unit
// by the blitter temporaries of XSurface::Copy_From.
