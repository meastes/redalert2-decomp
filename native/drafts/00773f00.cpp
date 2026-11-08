// NON-EXACT DRAFT 0x773F00 FUN_00773f00
// entry 0x773F00; exact-match score 99.44 (misses_index best), this text 98.91; plan "draft as-is"
// origin lane: Scout848 (build/class-work/Scout848/w/pg.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// bool XSurface::Plot_Line (0x00773F00, slot 17)
#include "shared/surface.hpp"
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
        int x2 = end.X;
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
