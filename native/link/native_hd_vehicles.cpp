// HD vehicles (native addition, not in the original; lane C16, user 2026-11-14: the
// Enhancements page's "HD vehicles", Off or 2x, off by default and in the Original preset).
// Written from the behaviour the redalert2-recomp design notes describe (docs/voxels.md: the
// sub-pixel passes of the voxel finish stage, a 2x image that follows the unit's own 1x pixels,
// shown where the picture still shows them); no code taken from it, and the mechanism here is
// our own: the game's draw path is recovered C++, so the 2x image is made by the game's own
// code run again at 2x, not learnt from the 1x picture.
//
// The game draws exactly what it drew before; the 2x image exists beside it:
//   * VoxelDrawSystem::Render 0x007178E0 draws a voxel object (or its shadow) into the
//     256x256 voxel buffer. With HD on, three more passes draw it again, each starting half
//     a pixel further left, up, or both (VoxelLibrary::Render_Object / Render_Shadow start
//     offsets, Native_Hd_Voxel_Dx/Dy); the four 1x images interleaved are the 2x image of the
//     same object (each pass is a whole 1x image, so there are no holes). The buffer and its
//     depth buffer get their 1x contents back.
//   * Every Bit_Blit 0x00434B40 whose source holds a 2x image (the voxel buffer, then the
//     unit's 256x256 staging surface, then the battlefield, then the screen) runs the same
//     blitter (the house colours, lighting, shadow darkening) over the 2x rows into the
//     destination's 2x image, for the destination pixels the 1x blit changed (the depth
//     buffer's say: a pixel the 1x blit did not write is not taken) and the transparent ones
//     beside them (the 2x edge). DirectDraw copies (DSurface::Copy_From) carry it along.
//   * Each 2x pixel group is stamped with the 1x value it belongs to; it counts only while the
//     surface still holds that value there, so whatever is drawn later over a unit (a health
//     bar, smoke, the cursor) shows at 1x, and nothing needs clearing.
//   * The host (platform/sdl/host.cpp) lays the screen's 2x pixels over the picture where they
//     still count, at the picture's scale: with Map zoom the units are genuinely sharper.
// The game's voxel and shadow caches are off while HD is on (cached images have no 2x image):
// the game's own DisableVoxelCache / DisableShadowCache behaviour. The setting is taken when a
// game starts or loads, so a cache filled with it off is never used with it on.
#include "shared/surface.hpp"
#include "shared/rect.hpp"
#include "shared/blit.hpp"
#include "shared/blitter.hpp"
#include "shared/vector3.hpp"
#include "shared/voxel_draw_system.hpp"
#include "shared/voxel_render_record.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

bool __fastcall Bit_Blit_Core_Native(Surface &dest, Rect const &destwindow, Rect const &destarea, Surface &source,
                                     Rect const &sourcewindow, Rect const &sourcearea, Blitter const &blitter,
                                     int depth, int ramp_index, int alpha, int unknown);   // blit.cpp (native fixup)
void Native_Hd_Voxel_Replay(Vector3 const &center, UnknownVoxelRecord88 **render_data);    // voxdrsys.cpp (native fixup)
extern unsigned char unknown_buffer_ae3408[256 * 256];      // 0x00AE3408 the voxel draw buffer
extern unsigned char unknown_zbuffer_ad0a70[256 * 256];     // 0x00AD0A70 its depth buffer
extern Surface *unknown_surface_839950;                     // 0x00839950 the screen
extern int unknown_voxel_record_count_ae0cb0;               // 0x00AE0CB0 objects queued for the draw
extern int unknown_voxel_shadow_count_ae3000;               // 0x00AE3000 shadows queued for the draw

extern "C" int ra2port_last_frame(void);                   // port: the last Main_Loop frame

// The setting (platform/sdl/enhancements.cpp): 0 off, 2 for 2x. Weak default: off.
extern "C" __attribute__((weak)) int ra2_enh_hd_vehicles(void) { return 0; }

int Native_Hd_Voxel_Dx = 0, Native_Hd_Voxel_Dy = 0;   // the passes' start offsets (8.8 fixed point)

namespace {

// A growable buffer (the link units are built in the game's dialect, without the C++ library).
template <class T>
struct Buf {
    T *p = nullptr;
    size_t n = 0;
    void resize(size_t count)
    {
        if (count <= n) return;
        p = (T *)realloc(p, count * sizeof(T));
        n = count;
    }
    void assign(size_t count, T value)
    {
        free(p);
        p = (T *)malloc(count * sizeof(T));
        n = count;
        for (size_t i = 0; i < count; ++i) p[i] = value;
    }
    T &operator[](size_t i) { return p[i]; }
    T const &operator[](size_t i) const { return p[i]; }
    T *data() { return p; }
};

bool active = false;             // taken when a game starts or loads
const bool stats = getenv("RA2_NATIVE_HD_STATS") != nullptr;
struct Stats { long renders, blits, pixels, shown, mismatched, drops; } counts = {0, 0, 0, 0, 0, 0};
// Time spent on the 2x work (microseconds; RA2_NATIVE_HD_STATS logs it per frame).
long long busy_us = 0, stage_us[4] = {0, 0, 0, 0};   // all; passes, blits, copies, overlay
long long now_us()
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (long long)t.tv_sec * 1000000 + t.tv_nsec / 1000;
}
struct Timed {
    int stage;
    long long start = now_us();
    explicit Timed(int s) : stage(s) {}
    ~Timed()
    {
        long long d = now_us() - start;
        busy_us += d;
        stage_us[stage] += d;
    }
};

// A surface's 2x image: hd (2w x 2h) and, per 1x pixel, the 1x value it was made for.
struct Layer {
    Surface *surface = nullptr;
    int w = 0, h = 0, bpp = 0;
    Buf<uint16_t> hd, stamp;
    Buf<uint8_t> valid;
    int x0 = 0, y0 = 0, x1 = 0, y1 = 0;      // the box holding every valid pixel (empty: x0 >= x1)
    // The areas holding valid pixels (the box is their union): a few units' rectangles, so copies and
    // the host's overlay work over them, not over the whole box between them.
    enum { kAreas = 64 };
    int areas = 0, area[kAreas][4];
    bool empty() const { return x0 >= x1 || y0 >= y1; }
    void clear_box()
    {
        for (int k = 0; k < areas; ++k)
            for (int y = area[k][1]; y < area[k][3]; ++y) memset(&valid[(size_t)y * w + area[k][0]], 0, area[k][2] - area[k][0]);
        areas = 0;
        x0 = y0 = x1 = y1 = 0;
    }
    void grow(int x, int y, int xe, int ye)
    {
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (xe > w) xe = w;
        if (ye > h) ye = h;
        if (x >= xe || y >= ye) return;
        if (empty()) { x0 = x; y0 = y; x1 = xe; y1 = ye; }
        else {
            if (x < x0) x0 = x;
            if (y < y0) y0 = y;
            if (xe > x1) x1 = xe;
            if (ye > y1) y1 = ye;
        }
        // Into an area it overlaps or touches, else a new one; when they run out, the box.
        for (int k = 0; k < areas; ++k) {
            int *a = area[k];
            if (x <= a[2] && xe >= a[0] && y <= a[3] && ye >= a[1]) {
                if (x < a[0]) a[0] = x;
                if (y < a[1]) a[1] = y;
                if (xe > a[2]) a[2] = xe;
                if (ye > a[3]) a[3] = ye;
                return;
            }
        }
        if (areas == kAreas) {
            areas = 1;
            area[0][0] = x0; area[0][1] = y0; area[0][2] = x1; area[0][3] = y1;
            return;
        }
        int *a = area[areas++];
        a[0] = x; a[1] = y; a[2] = xe; a[3] = ye;
    }
    // The copied area (x, y, xe, ye) loses its valid pixels; areas wholly inside it go.
    void drop(int x, int y, int xe, int ye)
    {
        int kept = 0;
        for (int k = 0; k < areas; ++k) {
            int *a = area[k];
            int ax = x > a[0] ? x : a[0], ay = y > a[1] ? y : a[1], bx = xe < a[2] ? xe : a[2], by = ye < a[3] ? ye : a[3];
            for (int row = ay; row < by && ax < bx; ++row) memset(&valid[(size_t)row * w + ax], 0, bx - ax);
            if (a[0] >= x && a[1] >= y && a[2] <= xe && a[3] <= ye) continue;
            if (kept != k) memcpy(area[kept], a, sizeof area[0]);
            ++kept;
        }
        areas = kept;
        if (!areas) x0 = y0 = x1 = y1 = 0;
    }
};
Layer *layers[16];
int layer_count = 0;

Layer *find(Surface *s)
{
    for (int i = 0; i < layer_count; ++i)
        if (layers[i]->surface == s) return layers[i];
    return nullptr;
}
Layer *make(Surface *s)
{
    int w = s->Get_Width(), h = s->Get_Height(), bpp = s->Bytes_Per_Pixel();
    Layer *l = find(s);
    if (l && l->w == w && l->h == h && l->bpp == bpp) return l;
    if (!l) {
        if (layer_count == 16) return nullptr;
        l = new Layer;
        layers[layer_count++] = l;
    }
    l->surface = s;
    l->w = w;
    l->h = h;
    l->bpp = bpp;
    l->hd.assign((size_t)w * h * 4, 0);
    l->stamp.assign((size_t)w * h, 0);
    l->valid.assign((size_t)w * h, 0);
    l->x0 = l->y0 = l->x1 = l->y1 = 0;
    return l;
}
// A layer that may hold something for the 1x size it was made for.
Layer *live(Surface *s)
{
    Layer *l = find(s);
    if (!l || l->empty()) return nullptr;
    if (l->w != s->Get_Width() || l->h != s->Get_Height() || l->bpp != s->Bytes_Per_Pixel()) return nullptr;
    return l;
}

// A 1x surface read through its lock: base pointer, stride, pixel size.
struct Pixels {
    unsigned char *base = nullptr;
    int stride = 0, bpp = 0;
    uint16_t at(int x, int y) const
    {
        unsigned char *p = base + (size_t)y * stride + (size_t)x * bpp;
        return bpp == 1 ? *p : *(uint16_t *)p;
    }
};
bool lock(Surface &s, Pixels &p)
{
    p.base = (unsigned char *)s.Lock(Point2D(0, 0));
    if (!p.base) return false;
    p.stride = s.Stride();
    p.bpp = s.Bytes_Per_Pixel();
    return true;
}

// The 2x pixel (X, Y) of a surface: its layer's where that still counts, else its 1x pixel.
inline uint16_t hd_at(Layer const *l, Pixels const &p, int X, int Y)
{
    int x = X >> 1, y = Y >> 1;
    uint16_t v = p.at(x, y);
    if (l && x >= l->x0 && x < l->x1 && y >= l->y0 && y < l->y1) {
        size_t i = (size_t)y * l->w + x;
        if (l->valid[i] && l->stamp[i] == v) return l->hd[(size_t)Y * (l->w * 2) + X];
    }
    return v;
}

// Scratch rows for the 2x blit (2x the widest blit) and a depth/alpha row the blitter may read and
// write: the start of the game's own buffers, put back after each row.
Buf<unsigned char> src_row, dst_row;
Buf<uint16_t> keep_depth, keep_alpha;

// The row a 2x blit row reads its depth or alpha values from: the game's buffer values at the 1x
// destination pixels (x, y) .. (x + w - 1, y), each twice, written at the buffer's start (what was there
// is kept in keep and put back by row_restore). The 2x blit then sees the light (the alpha buffer) the
// 1x blit saw at each pixel.
Buf<uint16_t> row_values;
template <class Buffer>
unsigned short *row_start(Buffer *b, int x, int y, int w, Buf<uint16_t> &keep)
{
    if (!b || !b->BufferHead) return nullptr;
    int length = 2 * w;
    row_values.resize(length);
    unsigned short *v = b->Get_Buffer_Offset(Point2D(x, y - b->Area.Y));
    for (int i = 0; i < w; ++i) {
        row_values[2 * i] = row_values[2 * i + 1] = *v;
        v = b->Wrap_Pointer(v + 1);
    }
    unsigned short *p = (unsigned short *)b->BufferHead;
    keep.resize(length);
    unsigned short *q = p;
    for (int i = 0; i < length; ++i) {
        keep[i] = *q;
        *q = row_values[i];
        q = b->Wrap_Pointer(q + 1);
    }
    return p;
}
template <class Buffer>
void row_restore(Buffer *b, unsigned short *p, Buf<uint16_t> const &keep, int length)
{
    if (!p) return;
    for (int i = 0; i < length; ++i) {
        *p = keep[i];
        p = b->Wrap_Pointer(p + 1);
    }
}

// The plain copies (XSurface::Copy_From's software path): a copy carries the 2x pixels as they are.
bool is_plain(Blitter const &blitter)
{
    static BlitPlain<unsigned short> plain16;
    static BlitPlain<unsigned char> plain8;
    void *vt = *(void *const *)&blitter;
    return vt == *(void **)&plain16 || vt == *(void **)&plain8;
}

// A plain copy of (sx, sy, w, h) of source to (dx, dy) of dest, after the 1x copy: the 2x pixels that
// counted in the source count in the dest; the rest of the copied area loses its own.
void copy_layer(Surface &dest, int dx, int dy, Surface &source, int sx, int sy, int w, int h)
{
    Layer *dl = live(&dest), *sl = live(&source);
    if (!sl && !dl) return;
    Timed timed(2);
    if (dl) dl->drop(dx, dy, dx + w, dy + h);     // the copied area takes the source's
    if (!sl) return;
    if (sx + w <= sl->x0 || sx >= sl->x1 || sy + h <= sl->y0 || sy >= sl->y1) return;
    Pixels sp, dp;
    if (!lock(source, sp)) return;
    if (!lock(dest, dp)) {
        source.Unlock();
        return;
    }
    dl = make(&dest);
    if (!dl) {
        dest.Unlock();
        source.Unlock();
        return;
    }
    int ox = dx - sx, oy = dy - sy;
    for (int k = 0; k < sl->areas; ++k) {
        int *a = sl->area[k];
        int ax = sx > a[0] ? sx : a[0], ay = sy > a[1] ? sy : a[1];
        int bx = sx + w < a[2] ? sx + w : a[2], by = sy + h < a[3] ? sy + h : a[3];
        if (ax + ox < 0) ax = -ox;
        if (ay + oy < 0) ay = -oy;
        if (bx + ox > dl->w) bx = dl->w - ox;
        if (by + oy > dl->h) by = dl->h - oy;
        if (ax >= bx || ay >= by) continue;
        bool any = false;
        for (int y = ay; y < by; ++y) {
            int ty = y + oy;
            for (int x = ax; x < bx; ++x) {
                size_t si = (size_t)y * sl->w + x;
                if (!sl->valid[si] || sl->stamp[si] != sp.at(x, y)) continue;
                int tx = x + ox;
                size_t di = (size_t)ty * dl->w + tx;
                dl->valid[di] = 1;
                dl->stamp[di] = dp.at(tx, ty);
                size_t s0 = (size_t)(2 * y) * (sl->w * 2) + 2 * x, d0 = (size_t)(2 * ty) * (dl->w * 2) + 2 * tx;
                dl->hd[d0] = sl->hd[s0];
                dl->hd[d0 + 1] = sl->hd[s0 + 1];
                dl->hd[d0 + dl->w * 2] = sl->hd[s0 + sl->w * 2];
                dl->hd[d0 + dl->w * 2 + 1] = sl->hd[s0 + sl->w * 2 + 1];
                any = true;
            }
        }
        if (any) dl->grow(ax + ox, ay + oy, bx + ox, by + oy);
    }
    dest.Unlock();
    source.Unlock();
}

// A shadow's 2x image (its buffer holds 1 where the shadow falls): the four passes interleaved leave
// a checkered edge where they part; a closing (3x3 dilation, then erosion) makes it a solid edge at
// 2x. The 1x render's own pixels (each group's top left) stay as drawn.
void close_shadow(Layer *l, int x0, int y0, int x1, int y1)
{
    int W = l->w * 2, X0 = 2 * x0, Y0 = 2 * y0, X1 = 2 * x1, Y1 = 2 * y1, w = X1 - X0, h = Y1 - Y0;
    static Buf<unsigned char> mask, grown;
    mask.resize((size_t)w * h);
    grown.resize((size_t)w * h);
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x) mask[(size_t)y * w + x] = l->hd[(size_t)(Y0 + y) * W + X0 + x] != 0;
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x) {
            unsigned char any = 0;
            for (int j = y - 1; j <= y + 1 && !any; ++j)
                for (int i = x - 1; i <= x + 1; ++i)
                    if (i >= 0 && j >= 0 && i < w && j < h && mask[(size_t)j * w + i]) { any = 1; break; }
            grown[(size_t)y * w + x] = any;
        }
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x) {
            if (((X0 + x) & 1) == 0 && ((Y0 + y) & 1) == 0) continue;     // the 1x render's own pixel
            unsigned char all = 1;
            for (int j = y - 1; j <= y + 1 && all; ++j)
                for (int i = x - 1; i <= x + 1; ++i)
                    if (i < 0 || j < 0 || i >= w || j >= h || !grown[(size_t)j * w + i]) { all = 0; break; }
            if (all && !mask[(size_t)y * w + x]) l->hd[(size_t)(Y0 + y) * W + X0 + x] = 1;
        }
}

}  // namespace

// Whether the game in play draws HD vehicles (the voxel caches are off then: techno.cpp fixups).
bool Native_Hd_Vehicles_Active() { return active; }

// A game starts or loads (link/native_map_zoom.cpp, beside the map zoom): it takes the setting now.
void Native_Hd_Vehicles_Start()
{
    bool on = ra2_enh_hd_vehicles() >= 2;
    if (on != active) fprintf(stderr, "hd-vehicles: %s for this game\n", on ? "2x" : "off");
    active = on;
    for (int i = 0; i < layer_count; ++i) layers[i]->clear_box();
}

// VoxelDrawSystem::Render 0x007178E0, after its 1x draw (native fixup): the three half-pixel passes.
void Native_Hd_Voxel_Passes(Rect const &rect, Vector3 const &center, UnknownVoxelRecord88 **render_data)
{
    Surface *grid = VoxelDrawSystem::Get_Surface();
    if (!active || !grid) return;
    Timed timed(0);
    Layer *l = make(grid);
    if (!l || l->w != 256 || l->h != 256 || l->bpp != 1) return;
    l->clear_box();
    // The passes' area: the drawn rectangle and a margin (a pass lands up to a pixel further).
    int x0 = rect.X - 2, y0 = rect.Y - 2, x1 = rect.X + rect.Width + 3, y1 = rect.Y + rect.Height + 3;
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 > 256) x1 = 256;
    if (y1 > 256) y1 = 256;
    if (x0 >= x1 || y0 >= y1) return;
    int w = x1 - x0;
    static Buf<unsigned char> keep_grid, keep_z;
    keep_grid.resize((size_t)w * (y1 - y0));
    keep_z.resize((size_t)w * (y1 - y0));
    for (int y = y0; y < y1; ++y) {
        memcpy(&keep_grid[(size_t)(y - y0) * w], &unknown_buffer_ae3408[y * 256 + x0], w);
        memcpy(&keep_z[(size_t)(y - y0) * w], &unknown_zbuffer_ad0a70[y * 256 + x0], w);
    }
    int W = l->w * 2;
    for (int y = y0; y < y1; ++y)
        for (int x = x0; x < x1; ++x) l->hd[(size_t)(2 * y) * W + 2 * x] = unknown_buffer_ae3408[y * 256 + x];
    for (int pass = 1; pass < 4; ++pass) {
        int ax = pass & 1, ay = pass >> 1;
        for (int y = y0; y < y1; ++y) {
            memset(&unknown_buffer_ae3408[y * 256 + x0], 0, w);
            memset(&unknown_zbuffer_ad0a70[y * 256 + x0], 0, w);
        }
        Native_Hd_Voxel_Dx = -128 * ax;
        Native_Hd_Voxel_Dy = -128 * ay;
        Native_Hd_Voxel_Replay(center, render_data);
        for (int y = y0; y < y1; ++y)
            for (int x = x0; x < x1; ++x)
                l->hd[(size_t)(2 * y + ay) * W + 2 * x + ax] = unknown_buffer_ae3408[y * 256 + x];
    }
    Native_Hd_Voxel_Dx = Native_Hd_Voxel_Dy = 0;
    if (unknown_voxel_record_count_ae0cb0 == 0 && unknown_voxel_shadow_count_ae3000 > 0) close_shadow(l, x0, y0, x1, y1);
    for (int y = y0; y < y1; ++y) {
        memcpy(&unknown_buffer_ae3408[y * 256 + x0], &keep_grid[(size_t)(y - y0) * w], w);
        memcpy(&unknown_zbuffer_ad0a70[y * 256 + x0], &keep_z[(size_t)(y - y0) * w], w);
        for (int x = x0; x < x1; ++x) {
            size_t i = (size_t)y * 256 + x;
            l->stamp[i] = unknown_buffer_ae3408[i];
            l->valid[i] = 1;
        }
    }
    l->grow(x0, y0, x1, y1);
    ++counts.renders;
}

// Bit_Blit 0x00434B40 (the original renamed Bit_Blit_Core_Native by a native fixup).
bool __fastcall Bit_Blit(Surface &dest, Rect const &destwindow, Rect const &destarea, Surface &source,
                         Rect const &sourcewindow, Rect const &sourcearea, Blitter const &blitter,
                         int depth, int ramp_index, int alpha, int unknown)
{
    Layer *sl = active ? live(&source) : nullptr;
    Layer *dl0 = active ? live(&dest) : nullptr;
    if (!sl && !dl0)
        return Bit_Blit_Core_Native(dest, destwindow, destarea, source, sourcewindow, sourcearea, blitter, depth, ramp_index,
                                    alpha, unknown);
    // The areas the blit will touch (Prep_For_Blit's clipping).
    Rect d = destarea, s = sourcearea;
    bool clipped = d.Is_Valid() && destwindow.Is_Valid() && s.Is_Valid() && sourcewindow.Is_Valid()
                   && Clip_Blit_Rects(d, destwindow, s, sourcewindow);
    if (!clipped)
        return Bit_Blit_Core_Native(dest, destwindow, destarea, source, sourcewindow, sourcearea, blitter, depth, ramp_index,
                                    alpha, unknown);
    int dx = destwindow.X + d.X, dy = destwindow.Y + d.Y, sx = sourcewindow.X + s.X, sy = sourcewindow.Y + s.Y;
    int rows = s.Height < d.Height ? s.Height : d.Height, cols = s.Width;
    if (dx + cols > dest.Get_Width()) cols = dest.Get_Width() - dx;
    if (is_plain(blitter) || !sl) {
        bool ok = Bit_Blit_Core_Native(dest, destwindow, destarea, source, sourcewindow, sourcearea, blitter, depth, ramp_index,
                                       alpha, unknown);
        if (ok && is_plain(blitter)) copy_layer(dest, dx, dy, source, sx, sy, cols, rows);
        return ok;
    }
    // Only the part of the blit over the source's 2x pixels is worked at 2x.
    int ax = sx > sl->x0 ? sx : sl->x0, ay = sy > sl->y0 ? sy : sl->y0;
    int bx = sx + cols < sl->x1 ? sx + cols : sl->x1, by = sy + rows < sl->y1 ? sy + rows : sl->y1;
    if (ax >= bx || ay >= by || dest.Bytes_Per_Pixel() > 2)
        return Bit_Blit_Core_Native(dest, destwindow, destarea, source, sourcewindow, sourcearea, blitter, depth, ramp_index,
                                    alpha, unknown);
    Timed timed(1);
    int ox = dx - sx, oy = dy - sy;            // source to destination
    int w = bx - ax, h = by - ay;
    // The destination before the blit.
    static Buf<uint16_t> before;
    before.resize((size_t)w * h);
    Pixels dp;
    if (!lock(dest, dp))
        return Bit_Blit_Core_Native(dest, destwindow, destarea, source, sourcewindow, sourcearea, blitter, depth, ramp_index,
                                    alpha, unknown);
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x) before[(size_t)y * w + x] = dp.at(ax + ox + x, ay + oy + y);
    dest.Unlock();
    bool ok = Bit_Blit_Core_Native(dest, destwindow, destarea, source, sourcewindow, sourcearea, blitter, depth, ramp_index,
                                   alpha, unknown);
    if (!ok) return ok;
    Pixels sp;
    if (!lock(source, sp)) return ok;
    if (!lock(dest, dp)) {
        source.Unlock();
        return ok;
    }
    Layer *dl = make(&dest);
    if (!dl) {
        dest.Unlock();
        source.Unlock();
        return ok;
    }
    int dbpp = dp.bpp, sbpp = sp.bpp;
    // The 2x rows: the source's 2x pixels through the same blitter onto the destination's 2x pixels as
    // they were (its own where they counted, else its 1x pixel), every depth test passing (the 1x blit
    // has decided which pixels the unit takes).
    int W2 = 2 * w;
    src_row.resize((size_t)W2 * 2);
    dst_row.resize((size_t)W2 * 2);
    static Buf<uint16_t> out;
    static Buf<unsigned char> chg;
    out.resize((size_t)W2 * 2 * h);
    chg.resize((size_t)w * h);
    int SW = sl->w, DW = dl0 ? dl0->w : 0;
    for (int y = 0; y < h; ++y) {
        // This 1x row's 2x pixels, source and destination (as they were), for both 2x rows: a layer's
        // 2x pixel where it still counts for the 1x value, else the 1x value.
        int sy1 = ay + y, dy1 = ay + oy + y;
        const unsigned char *s1 = sp.base + (size_t)sy1 * sp.stride + (size_t)ax * sbpp;
        const uint16_t *b1 = &before[(size_t)y * w];
        bool sin = sy1 >= sl->y0 && sy1 < sl->y1;
        bool din = dl0 && dy1 >= dl0->y0 && dy1 < dl0->y1;
        for (int half = 0; half < 2; ++half) {
            unsigned char *srow8 = src_row.data();
            uint16_t *srow16 = (uint16_t *)src_row.data();
            const uint16_t *shd = &sl->hd[(size_t)(2 * sy1 + half) * (SW * 2) + 2 * ax];
            const unsigned char *sval = &sl->valid[(size_t)sy1 * SW + ax];
            const uint16_t *sst = &sl->stamp[(size_t)sy1 * SW + ax];
            for (int x = 0; x < w; ++x) {
                uint16_t v = sbpp == 1 ? s1[x] : ((const uint16_t *)s1)[x];
                uint16_t a = v, b = v;
                int gx = ax + x;
                if (sin && gx >= sl->x0 && gx < sl->x1 && sval[x] && sst[x] == v) {
                    a = shd[2 * x];
                    b = shd[2 * x + 1];
                }
                if (sbpp == 1) { srow8[2 * x] = (unsigned char)a; srow8[2 * x + 1] = (unsigned char)b; }
                else { srow16[2 * x] = a; srow16[2 * x + 1] = b; }
            }
            unsigned char *drow8 = dst_row.data();
            uint16_t *drow16 = dbpp == 2 ? &out[(size_t)(2 * y + half) * W2] : nullptr;
            const uint16_t *dhd = din ? &dl0->hd[(size_t)(2 * dy1 + half) * (DW * 2) + 2 * (ax + ox)] : nullptr;
            const unsigned char *dval = din ? &dl0->valid[(size_t)dy1 * DW + ax + ox] : nullptr;
            const uint16_t *dst_ = din ? &dl0->stamp[(size_t)dy1 * DW + ax + ox] : nullptr;
            for (int x = 0; x < w; ++x) {
                uint16_t v = b1[x], a = v, b = v;
                int gx = ax + ox + x;
                if (din && gx >= dl0->x0 && gx < dl0->x1 && dval[x] && dst_[x] == v) {
                    a = dhd[2 * x];
                    b = dhd[2 * x + 1];
                }
                if (dbpp == 1) { drow8[2 * x] = (unsigned char)a; drow8[2 * x + 1] = (unsigned char)b; }
                else { drow16[2 * x] = a; drow16[2 * x + 1] = b; }
            }
            void *target = dbpp == 2 ? (void *)drow16 : (void *)drow8;
            unsigned short *zrow = row_start(DepthBuffer, ax + ox, dy1, w, keep_depth);
            unsigned short *arow = row_start(AlphaBuffer, ax + ox, dy1, w, keep_alpha);
            blitter.BlitForward(target, src_row.data(), W2, -1, zrow, arow, alpha, 0);
            row_restore(DepthBuffer, zrow, keep_depth, W2);
            row_restore(AlphaBuffer, arow, keep_alpha, W2);
            if (dbpp == 1) {
                uint16_t *o = &out[(size_t)(2 * y + half) * W2];
                for (int X = 0; X < W2; ++X) o[X] = drow8[X];
            }
        }
        // Which 1x pixels the blit changed.
        const unsigned char *d1 = dp.base + (size_t)dy1 * dp.stride + (size_t)(ax + ox) * dbpp;
        for (int x = 0; x < w; ++x) {
            uint16_t v = dbpp == 1 ? d1[x] : ((const uint16_t *)d1)[x];
            chg[(size_t)y * w + x] = v != b1[x];
        }
    }
    // The pixels the 1x blit changed, and transparent ones beside them, take their 2x pixels.
    bool any = false;
    int gx0 = 1 << 30, gy0 = 1 << 30, gx1 = -1, gy1 = -1;
    auto changed = [&](int x, int y) { return x >= 0 && y >= 0 && x < w && y < h && chg[(size_t)y * w + x]; };
    for (int y = 0; y < h; ++y) {
        int ty = ay + oy + y;
        if (ty < 0 || ty >= dl->h) continue;
        for (int x = 0; x < w; ++x) {
            int tx = ax + ox + x;
            if (tx < 0 || tx >= dl->w) continue;
            bool take = chg[(size_t)y * w + x];
            if (!take && sp.at(ax + x, ay + y) == 0)
                take = changed(x - 1, y) || changed(x + 1, y) || changed(x, y - 1) || changed(x, y + 1);
            if (!take) continue;
            if (stats && changed(x, y) && out[(size_t)(2 * y) * W2 + 2 * x] != dp.at(tx, ty)) {
                static int told = 0;
                if (told++ < 20)
                    fprintf(stderr, "hd-vehicles: blit %d->%d bpp, 2x %04x 1x %04x before %04x src %02x src2x %04x vt %p\n", sbpp, dbpp,
                            out[(size_t)(2 * y) * W2 + 2 * x], dp.at(tx, ty), before[(size_t)y * w + x], sp.at(ax + x, ay + y),
                            hd_at(sl, sp, 2 * (ax + x), 2 * (ay + y)), *(void *const *)&blitter);
            }
            size_t di = (size_t)ty * dl->w + tx;
            dl->valid[di] = 1;
            dl->stamp[di] = dp.at(tx, ty);
            for (int k = 0; k < 4; ++k)
                dl->hd[(size_t)(2 * ty + (k >> 1)) * (dl->w * 2) + 2 * tx + (k & 1)] =
                    out[(size_t)(2 * y + (k >> 1)) * W2 + 2 * x + (k & 1)];
            if (tx < gx0) gx0 = tx;
            if (ty < gy0) gy0 = ty;
            if (tx >= gx1) gx1 = tx + 1;
            if (ty >= gy1) gy1 = ty + 1;
            any = true;
            ++counts.pixels;
        }
    }
    if (any) dl->grow(gx0, gy0, gx1, gy1);
    ++counts.blits;
    dest.Unlock();
    source.Unlock();
    return ok;
}

// DSurface::Copy_From's DirectDraw copy (native fixup): the 2x pixels follow the 1x ones.
void Native_Hd_Copied(Surface &dest, Rect const &destrect, Surface &source, Rect const &sourcerect)
{
    if (!active) return;
    int w = destrect.Width < sourcerect.Width ? destrect.Width : sourcerect.Width;
    int h = destrect.Height < sourcerect.Height ? destrect.Height : sourcerect.Height;
    copy_layer(dest, destrect.X, destrect.Y, source, sourcerect.X, sourcerect.Y, w, h);
}

// The host's overlay (platform/sdl/host.cpp): the screen's 2x pixels that still count in the picture
// it shows (pixels: the 16-bit screen, w x h, pitch in bytes) written opaque into argb (2w x 2h,
// 0xAARRGGBB), the rest of each area transparent with the picture's colour (a filtering scale then
// blends the overlay's edge with the right colour). The areas written (1x pixels: x, y, w, h) go into
// areas (at most max; *count of them); 0 when there is nothing (HD off, or no 2x pixels on the
// screen). The caller clears the areas it was given before.
extern "C" int ra2_hd_overlay(const void *pixels, int pitch, int w, int h, uint32_t *argb, int (*areas)[4], int max, int *count)
{
    *count = 0;
    if (!active || !unknown_surface_839950) return 0;
    Timed timed(3);
    Layer *l = live(unknown_surface_839950);
    if (!l || l->w != w || l->h != h || l->bpp != 2) return 0;
    int W = 2 * w;
    long shown = 0, mismatched = 0;
    for (int k = 0; k < l->areas && *count < max; ++k) {
        int *a = l->area[k];
        for (int y = a[1]; y < a[3]; ++y) {
            const uint16_t *row = (const uint16_t *)((const unsigned char *)pixels + (size_t)y * pitch);
            uint32_t *out0 = argb + (size_t)(2 * y) * W, *out1 = out0 + W;
            for (int x = a[0]; x < a[2]; ++x) {
                size_t i = (size_t)y * w + x;
                uint16_t c = row[x];
                bool show = l->valid[i] && l->stamp[i] == c;
                if (!show) {
                    unsigned r = (c >> 11) & 31, g = (c >> 5) & 63, b = c & 31;
                    uint32_t clear = ((r << 3 | r >> 2) << 16) | ((g << 2 | g >> 4) << 8) | (b << 3 | b >> 2);
                    out0[2 * x] = out0[2 * x + 1] = out1[2 * x] = out1[2 * x + 1] = clear;
                    continue;
                }
                const uint16_t *h0 = &l->hd[(size_t)(2 * y) * W + 2 * x], *h1 = h0 + W;
                uint16_t q[4] = {h0[0], h0[1], h1[0], h1[1]};
                uint32_t *o[4] = {&out0[2 * x], &out0[2 * x + 1], &out1[2 * x], &out1[2 * x + 1]};
                for (int k2 = 0; k2 < 4; ++k2) {
                    unsigned r = (q[k2] >> 11) & 31, g = (q[k2] >> 5) & 63, b = q[k2] & 31;
                    *o[k2] = 0xFF000000u | ((r << 3 | r >> 2) << 16) | ((g << 2 | g >> 4) << 8) | (b << 3 | b >> 2);
                }
                // The 2x group's top-left pixel is the 1x render's own (pass 0): it must be the 1x pixel.
                if (q[0] != c) ++mismatched;
                ++shown;
            }
        }
        areas[*count][0] = a[0];
        areas[*count][1] = a[1];
        areas[*count][2] = a[2] - a[0];
        areas[*count][3] = a[3] - a[1];
        ++*count;
    }
    // A flicker: a picture showing fewer than half the 2x unit pixels of the one before (from at
    // least 2,000), the units falling back to 1x for a picture.
    if (counts.shown >= 2000 && shown < counts.shown / 2) {
        ++counts.drops;
        if (stats)
            fprintf(stderr, "hd-vehicles: drop at frame %d: %ld screen pixels at 2x after %ld (%d areas)\n", ra2port_last_frame(),
                    shown, counts.shown, l->areas);
    }
    counts.shown = shown;
    counts.mismatched = mismatched;
    static int calls = 0;
    static long long busy_before = 0;
    if (stats && ++calls % 100 == 0) {
        fprintf(stderr, "hd-vehicles: renders %ld blits %ld pixels %ld; shown %ld, mismatched %ld; 2x work %.2f ms a picture; "
                "drops %ld (passes %.2f, blits %.2f, copies %.2f, overlay %.2f)\n", counts.renders, counts.blits, counts.pixels, shown,
                mismatched, (busy_us - busy_before) / 100000.0, counts.drops, stage_us[0] / 100000.0, stage_us[1] / 100000.0,
                stage_us[2] / 100000.0, stage_us[3] / 100000.0);
        busy_before = busy_us;
        stage_us[0] = stage_us[1] = stage_us[2] = stage_us[3] = 0;
    }
    return shown > 0;
}

// The counts (tests: RA2_NATIVE_HD_STATS=1 logs them each 100 frames, harness_input notes them).
extern "C" void ra2_hd_counts(long *renders, long *blits, long *pixels, long *shown, long *mismatched)
{
    *renders = counts.renders;
    *blits = counts.blits;
    *pixels = counts.pixels;
    *shown = counts.shown;
    *mismatched = counts.mismatched;
}
