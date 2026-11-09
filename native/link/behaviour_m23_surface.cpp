// Behaviour-written surface slots for native M23 (Scout1187). NOT EXACT: the painting
// is simplified (no depth buffer, no antialiased edges); each states what it keeps.
#include "shared/dsurface.hpp"
#include "shared/hicolor.hpp"

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
// (LaserDrawClass::Draw_It 0x005334F0: Prism towers and laser weapons). The
// original adds the colour, scaled by intensity, to the pixels along the line in
// front of the depth buffer with antialiased edges. Simplified: the colour scaled
// by intensity is added (saturating) to each channel of the pixels on the line
// inside the clip rectangle; no depth test, no edge falloff.
bool DSurface::Draw_Depth_Antialiased_Line(Rect const &cliprect, Point2D const &startpoint, Point2D const &endpoint,
                                           RGBClass const &color, int, int, bool, bool, bool, bool, float intensity)
{
    if (intensity <= 0.0f) {
        return false;
    }
    int red = (int)(color.Red * intensity), green = (int)(color.Green * intensity), blue = (int)(color.Blue * intensity);
    int x0 = startpoint.X, y0 = startpoint.Y, x1 = endpoint.X, y1 = endpoint.Y;
    int dx = x1 > x0 ? x1 - x0 : x0 - x1, sx = x0 < x1 ? 1 : -1;
    int dy = y1 > y0 ? y0 - y1 : y1 - y0, sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    for (int steps = 0; steps < 8192; ++steps) {
        if (Inside(cliprect, x0, y0)) {
            Point2D p;
            p.X = x0;
            p.Y = y0;
            RGBClass back = Unpack_Hicolor_Pixel((unsigned short)Get_Pixel(p));
            int r = back.Red + red, g = back.Green + green, b = back.Blue + blue;
            Put_Pixel(p, Build_Hicolor_Pixel(r > 255 ? 255 : r, g > 255 ? 255 : g, b > 255 ? 255 : b));
        }
        if (x0 == x1 && y0 == y1) {
            break;
        }
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
    return true;
}
