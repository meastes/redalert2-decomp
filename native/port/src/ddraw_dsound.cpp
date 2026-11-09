// DirectDraw and DirectSound as C++ objects with the COM interface shapes the
// recovered code calls (shared/directdraw.hpp, shared/dsound_api.hpp: same slot
// order, same argument widths). Surfaces are system-memory pixel buffers; the
// primary surface is shown through the host on Flip/Blt/Unlock. Sound buffers
// keep their PCM; the playing secondary buffers are mixed (resampled to the
// output rate by their frequency, scaled by volume and pan, 8/16-bit, mono or
// stereo) into the host's audio stream (Host::open_audio: an SDL3 stream), and
// their play cursors advance by what the device consumed. Without a host audio
// device the cursors advance by wall time and the buffers play silently.
#include "ra2port/win32_port.hpp"

#include <chrono>
#include <math.h>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace ra2port;

namespace {

const HRESULT DD_OK = 0, E_NOTIMPL = (HRESULT)0x80004001u, DDERR_INVALIDPARAMS = (HRESULT)0x80070057u;

struct DDCOLORKEY { DWORD lo, hi; };
struct DDPIXELFORMAT { DWORD dwSize, dwFlags, dwFourCC, dwRGBBitCount, dwRBitMask, dwGBitMask, dwBBitMask, dwRGBAlphaBitMask; };
struct DDSCAPS { DWORD dwCaps; };
struct DDSURFACEDESC {
    DWORD dwSize, dwFlags, dwHeight, dwWidth;
    LONG lPitch;
    DWORD dwBackBufferCount, dwRefreshRate, dwAlphaBitDepth, dwReserved;
    void *lpSurface;
    DDCOLORKEY ddckCKDestOverlay, ddckCKDestBlt, ddckCKSrcOverlay, ddckCKSrcBlt;
    DDPIXELFORMAT ddpfPixelFormat;
    DDSCAPS ddsCaps;
};
struct DDBLTFX { DWORD dwSize; DWORD unknown_04[19]; DWORD dwFillColor; DWORD unknown_54[4]; };

const DWORD DDSCAPS_PRIMARYSURFACE = 0x200, DDSD_PIXELFORMAT = 0x1000, DDSD_PITCH = 0x8,
            DDBLT_COLORFILL = 0x400, DDBLT_KEYSRC = 0x8000, DDBLTFAST_SRCCOLORKEY = 1;

int display_width = 800, display_height = 600, display_bits = 16;

struct Clipper {
    virtual HRESULT QueryInterface(const void *, void **o) { *o = this; return DD_OK; }
    virtual ULONG AddRef() { return ++refs; }
    virtual ULONG Release() { ULONG r = --refs; if (!r) delete this; return r; }
    virtual HRESULT GetClipList(RECT *, void *, DWORD *) { return E_NOTIMPL; }
    virtual HRESULT GetHWnd(HWND *w) { *w = window; return DD_OK; }
    virtual HRESULT Initialize(void *, DWORD) { return DD_OK; }
    virtual HRESULT IsClipListChanged(BOOL *c) { *c = 0; return DD_OK; }
    virtual HRESULT SetClipList(void *, DWORD) { return DD_OK; }
    virtual HRESULT SetHWnd(DWORD, HWND w) { window = w; return DD_OK; }
    virtual ~Clipper() {}
    ULONG refs = 1;
    HWND window = nullptr;
};

struct Surface {
    virtual HRESULT QueryInterface(const void *, void **o) { *o = this; AddRef(); return DD_OK; }
    virtual ULONG AddRef() { return ++refs; }
    virtual ULONG Release() { ULONG r = --refs; if (!r) delete this; return r; }
    virtual HRESULT AddAttachedSurface(Surface *) { return DD_OK; }
    virtual HRESULT AddOverlayDirtyRect(RECT *) { return DD_OK; }
    virtual HRESULT Blt(RECT *dst, Surface *src, RECT *srcrect, DWORD flags, DDBLTFX *fx)
    {
        RECT d = dst ? *dst : RECT{0, 0, width, height};
        if (flags & DDBLT_COLORFILL) {
            fill(d, fx ? fx->dwFillColor : 0);
        } else if (src) {
            RECT s = srcrect ? *srcrect : RECT{0, 0, src->width, src->height};
            if (d.right - d.left != s.right - s.left || d.bottom - d.top != s.bottom - s.top) {
                stretch(d, src, s, (flags & DDBLT_KEYSRC) != 0);
            } else {
                copy(d.left, d.top, src, s, (flags & DDBLT_KEYSRC) != 0);
            }
        }
        if (primary) present();
        return DD_OK;
    }
    virtual HRESULT BltBatch(void *, DWORD, DWORD) { return E_NOTIMPL; }
    virtual HRESULT BltFast(DWORD x, DWORD y, Surface *src, RECT *srcrect, DWORD flags)
    {
        if (!src) return DDERR_INVALIDPARAMS;
        RECT s = srcrect ? *srcrect : RECT{0, 0, src->width, src->height};
        copy((LONG)x, (LONG)y, src, s, (flags & DDBLTFAST_SRCCOLORKEY) != 0);
        if (primary) present();
        return DD_OK;
    }
    virtual HRESULT DeleteAttachedSurface(DWORD, Surface *) { return DD_OK; }
    virtual HRESULT EnumAttachedSurfaces(void *, void *) { return E_NOTIMPL; }
    virtual HRESULT EnumOverlayZOrders(DWORD, void *, void *) { return E_NOTIMPL; }
    virtual HRESULT Flip(Surface *, DWORD) { present(); return DD_OK; }
    virtual HRESULT GetAttachedSurface(DDSCAPS *, Surface **s) { *s = this; AddRef(); return DD_OK; }
    virtual HRESULT GetBltStatus(DWORD) { return DD_OK; }
    virtual HRESULT GetCaps(DDSCAPS *c) { c->dwCaps = caps; return DD_OK; }
    virtual HRESULT GetClipper(Clipper **c) { *c = clipper; if (clipper) clipper->AddRef(); return DD_OK; }
    virtual HRESULT GetColorKey(DWORD, DDCOLORKEY *k) { k->lo = k->hi = colorkey; return DD_OK; }
    virtual HRESULT GetDC(HDC *dc) { *dc = (HDC)(uintptr_t)0x7000; return DD_OK; }
    virtual HRESULT GetFlipStatus(DWORD) { return DD_OK; }
    virtual HRESULT GetOverlayPosition(LONG *, LONG *) { return E_NOTIMPL; }
    virtual HRESULT GetPalette(void **) { return E_NOTIMPL; }
    virtual HRESULT GetPixelFormat(DDPIXELFORMAT *f) { pixel_format(f); return DD_OK; }
    virtual HRESULT GetSurfaceDesc(DDSURFACEDESC *d) { describe(d); return DD_OK; }
    virtual HRESULT Initialize(void *, DDSURFACEDESC *) { return DD_OK; }
    virtual HRESULT IsLost() { return DD_OK; }
    virtual HRESULT Lock(RECT *r, DDSURFACEDESC *d, DWORD, void *)
    {
        describe(d);
        if (r) d->lpSurface = pixels.data() + (size_t)r->top * pitch + (size_t)r->left * bytes;
        return DD_OK;
    }
    virtual HRESULT ReleaseDC(HDC) { return DD_OK; }
    virtual HRESULT Restore() { return DD_OK; }
    virtual HRESULT SetClipper(Clipper *c) { clipper = c; return DD_OK; }
    virtual HRESULT SetColorKey(DWORD, DDCOLORKEY *k) { colorkey = k ? k->lo : 0; return DD_OK; }
    virtual HRESULT SetOverlayPosition(LONG, LONG) { return E_NOTIMPL; }
    virtual HRESULT SetPalette(void *) { return DD_OK; }
    virtual HRESULT Unlock(void *) { if (primary) present(); return DD_OK; }
    // IDirectDrawSurface2/3 additions (QueryInterface returns this object).
    virtual HRESULT GetDDInterface(void **) { return E_NOTIMPL; }
    virtual HRESULT PageLock(DWORD) { return DD_OK; }
    virtual HRESULT PageUnlock(DWORD) { return DD_OK; }
    virtual HRESULT SetSurfaceDesc(DDSURFACEDESC *, DWORD) { return E_NOTIMPL; }
    virtual ~Surface() {}

    ULONG refs = 1;
    LONG width = 0, height = 0, pitch = 0, bytes = 2;
    // RA2_NATIVE_PRESENT_DUMP=<file.bmp>: every presented 16-bit (RGB565) frame
    // is written there as a 24-bit BMP (overwritten each time, so the file holds
    // the last frame; test runs only).
    void dump_presented()
    {
        const char *path = getenv("RA2_NATIVE_PRESENT_DUMP");
        if (!path || !*path || bytes != 2 || width <= 0 || height <= 0) return;
        // RA2_NATIVE_PRESENT_DUMP_EVERY=<n>: also keep every n-th presented frame as
        // <file>.<count>.bmp (test runs only).
        static long presents = 0;
        ++presents;
        char numbered[1024];
        const char *every = getenv("RA2_NATIVE_PRESENT_DUMP_EVERY");
        if (every && atol(every) > 0 && presents % atol(every) == 0) {
            snprintf(numbered, sizeof numbered, "%s.%06ld.bmp", path, presents);
            dump_to(numbered);
        }
        dump_to(path);
    }
    void dump_to(const char *path)
    {
        FILE *f = fopen(path, "wb");
        if (!f) return;
        uint32_t row = ((uint32_t)width * 3 + 3) & ~3u, size = 54 + row * (uint32_t)height;
        unsigned char h[54] = {'B', 'M'};
        auto put32 = [&](int at, uint32_t v) { for (int i = 0; i < 4; ++i) h[at + i] = (unsigned char)(v >> (8 * i)); };
        put32(2, size); put32(10, 54); put32(14, 40); put32(18, (uint32_t)width); put32(22, (uint32_t)height);
        h[26] = 1; h[28] = 24; put32(34, row * (uint32_t)height);
        fwrite(h, 1, 54, f);
        std::vector<unsigned char> line(row, 0);
        for (LONG y = height - 1; y >= 0; --y) {
            const uint16_t *src = (const uint16_t *)(pixels.data() + (size_t)y * pitch);
            for (LONG x = 0; x < width; ++x) {
                uint16_t c = src[x];
                line[x * 3 + 0] = (unsigned char)((c & 0x1F) << 3);
                line[x * 3 + 1] = (unsigned char)(((c >> 5) & 0x3F) << 2);
                line[x * 3 + 2] = (unsigned char)(((c >> 11) & 0x1F) << 3);
            }
            fwrite(line.data(), 1, row, f);
        }
        fclose(f);
    }
    DWORD caps = 0, colorkey = 0;
    bool primary = false;
    Clipper *clipper = nullptr;
    std::vector<unsigned char> pixels;

    void pixel_format(DDPIXELFORMAT *f)
    {
        memset(f, 0, sizeof *f);
        f->dwSize = sizeof *f;
        f->dwFlags = 0x40;   // DDPF_RGB
        f->dwRGBBitCount = (DWORD)bytes * 8;
        f->dwRBitMask = 0xF800;
        f->dwGBitMask = 0x07E0;
        f->dwBBitMask = 0x001F;
    }
    void describe(DDSURFACEDESC *d)
    {
        DWORD size = d->dwSize;
        memset(d, 0, sizeof *d);
        d->dwSize = size;
        d->dwFlags = 0x1 | 0x2 | 0x4 | DDSD_PITCH | DDSD_PIXELFORMAT;
        d->dwWidth = (DWORD)width;
        d->dwHeight = (DWORD)height;
        d->lPitch = pitch;
        d->lpSurface = pixels.data();
        d->ddsCaps.dwCaps = caps;
        pixel_format(&d->ddpfPixelFormat);
    }
    void fill(RECT r, DWORD color)
    {
        if (r.left < 0) r.left = 0;
        if (r.top < 0) r.top = 0;
        if (r.right > width) r.right = width;
        if (r.bottom > height) r.bottom = height;
        for (LONG y = r.top; y < r.bottom; ++y) {
            uint16_t *row = (uint16_t *)(pixels.data() + (size_t)y * pitch);
            for (LONG x = r.left; x < r.right; ++x) row[x] = (uint16_t)color;
        }
    }
    void copy(LONG dx, LONG dy, Surface *src, RECT s, bool keyed)
    {
        for (LONG y = s.top; y < s.bottom; ++y) {
            LONG ty = dy + (y - s.top);
            if (ty < 0 || ty >= height || y < 0 || y >= src->height) continue;
            const uint16_t *from = (const uint16_t *)(src->pixels.data() + (size_t)y * src->pitch);
            uint16_t *to = (uint16_t *)(pixels.data() + (size_t)ty * pitch);
            for (LONG x = s.left; x < s.right; ++x) {
                LONG tx = dx + (x - s.left);
                if (tx < 0 || tx >= width || x < 0 || x >= src->width) continue;
                if (keyed && from[x] == (uint16_t)src->colorkey) continue;
                to[tx] = from[x];
            }
        }
    }
    // Blt with destination and source rectangles of different sizes stretches
    // the source (DirectDraw's Blt; point sampling), clipped to the surface:
    // the map preview (MapPreviewClass::Draw 0x0061D460) is scaled into its frame.
    void stretch(RECT d, Surface *src, RECT s, bool keyed)
    {
        LONG dw = d.right - d.left, dh = d.bottom - d.top, sw = s.right - s.left, sh = s.bottom - s.top;
        if (dw <= 0 || dh <= 0 || sw <= 0 || sh <= 0) return;
        for (LONG y = 0; y < dh; ++y) {
            LONG ty = d.top + y, sy = s.top + (LONG)((int64_t)y * sh / dh);
            if (ty < 0 || ty >= height || sy < 0 || sy >= src->height) continue;
            const uint16_t *from = (const uint16_t *)(src->pixels.data() + (size_t)sy * src->pitch);
            uint16_t *to = (uint16_t *)(pixels.data() + (size_t)ty * pitch);
            for (LONG x = 0; x < dw; ++x) {
                LONG tx = d.left + x, sx = s.left + (LONG)((int64_t)x * sw / dw);
                if (tx < 0 || tx >= width || sx < 0 || sx >= src->width) continue;
                if (keyed && from[sx] == (uint16_t)src->colorkey) continue;
                to[tx] = from[sx];
            }
        }
    }
    void present()
    {
        if (host() && host()->present) host()->present(pixels.data(), (int)width, (int)height, (int)pitch, (int)bytes * 8);
        dump_presented();
        char detail[64];
        snprintf(detail, sizeof detail, "%dx%d %d bpp", (int)width, (int)height, (int)bytes * 8);
        progress("present", detail);
    }
};

typedef HRESULT (*EnumModesCallback)(DDSURFACEDESC *, void *);

struct DirectDraw {
    virtual HRESULT QueryInterface(const void *, void **o) { *o = this; AddRef(); return DD_OK; }
    virtual ULONG AddRef() { return ++refs; }
    virtual ULONG Release() { ULONG r = --refs; if (!r) delete this; return r; }
    virtual HRESULT Compact() { return DD_OK; }
    virtual HRESULT CreateClipper(DWORD, Clipper **c, void *) { *c = new Clipper; return DD_OK; }
    virtual HRESULT CreatePalette(DWORD, void *, void **p, void *) { *p = nullptr; return E_NOTIMPL; }
    virtual HRESULT CreateSurface(DDSURFACEDESC *d, Surface **out, void *)
    {
        Surface *s = new Surface;
        s->caps = d->ddsCaps.dwCaps;
        s->primary = (d->dwFlags & 1) && (d->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE);
        s->width = s->primary || !(d->dwFlags & 4) ? display_width : (LONG)d->dwWidth;
        s->height = s->primary || !(d->dwFlags & 2) ? display_height : (LONG)d->dwHeight;
        s->bytes = display_bits / 8;
        s->pitch = ((s->width * s->bytes) + 3) & ~3;
        s->pixels.assign((size_t)s->pitch * (size_t)s->height, 0);
        {
            char detail[96];
            snprintf(detail, sizeof detail, "%dx%d %d bpp caps 0x%x", (int)s->width, (int)s->height, (int)display_bits, (unsigned)s->caps);
            progress(s->primary ? "primary" : "surfaces", detail);
            progress_count("surfaces created", detail);
        }
        *out = s;
        return DD_OK;
    }
    virtual HRESULT DuplicateSurface(Surface *, Surface **) { return E_NOTIMPL; }
    virtual HRESULT EnumDisplayModes(DWORD, DDSURFACEDESC *, void *context, EnumModesCallback callback)
    {
        static const int modes[][2] = {{640, 480}, {800, 600}, {1024, 768}, {1280, 1024}};
        for (auto &m : modes) {
            DDSURFACEDESC d = {};
            d.dwSize = sizeof d;
            d.dwFlags = 0x1 | 0x2 | 0x4 | DDSD_PIXELFORMAT;
            d.dwWidth = (DWORD)m[0];
            d.dwHeight = (DWORD)m[1];
            d.lPitch = m[0] * 2;
            d.ddpfPixelFormat = {sizeof(DDPIXELFORMAT), 0x40, 0, 16, 0xF800, 0x07E0, 0x001F, 0};
            if (callback(&d, context) == 0) break;   // DDENUMRET_CANCEL
        }
        return DD_OK;
    }
    virtual HRESULT EnumSurfaces(DWORD, DDSURFACEDESC *, void *, void *) { return E_NOTIMPL; }
    virtual HRESULT FlipToGDISurface() { return DD_OK; }
    virtual HRESULT GetCaps(void *driver, void *emulation)
    {
        // DDCAPS is 0x13C bytes on Win32 (no pointers): report generous video memory.
        for (void *c : {driver, emulation}) {
            if (!c) continue;
            DWORD size = *(DWORD *)c;
            memset(c, 0, size ? size : 0x13C);
            *(DWORD *)c = size;
            ((DWORD *)c)[0x3C / 4] = 64u << 20;   // dwVidMemTotal
            ((DWORD *)c)[0x40 / 4] = 64u << 20;   // dwVidMemFree
        }
        return DD_OK;
    }
    virtual HRESULT GetDisplayMode(DDSURFACEDESC *d)
    {
        Surface s;
        s.width = display_width;
        s.height = display_height;
        s.bytes = display_bits / 8;
        s.pitch = s.width * s.bytes;
        s.describe(d);
        d->lpSurface = nullptr;
        return DD_OK;
    }
    virtual HRESULT GetFourCCCodes(DWORD *n, DWORD *) { if (n) *n = 0; return DD_OK; }
    virtual HRESULT GetGDISurface(Surface **) { return E_NOTIMPL; }
    virtual HRESULT GetMonitorFrequency(DWORD *f) { *f = 60; return DD_OK; }
    virtual HRESULT GetScanLine(DWORD *l) { *l = 0; return DD_OK; }
    virtual HRESULT GetVerticalBlankStatus(BOOL *b) { *b = 1; return DD_OK; }
    virtual HRESULT Initialize(void *) { return DD_OK; }
    virtual HRESULT RestoreDisplayMode() { return DD_OK; }
    virtual HRESULT SetCooperativeLevel(HWND, DWORD) { return DD_OK; }
    virtual HRESULT SetDisplayMode(DWORD w, DWORD h, DWORD bits)
    {
        display_width = (int)w;
        display_height = (int)h;
        display_bits = bits == 16 ? 16 : 16;   // the game draws in 16 bpp
        display_mode_changed(display_width, display_height);
        return DD_OK;
    }
    virtual HRESULT WaitForVerticalBlank(DWORD, void *) { return DD_OK; }
    // IDirectDraw2 addition.
    virtual HRESULT GetAvailableVidMem(DDSCAPS *, DWORD *total, DWORD *free_)
    {
        if (total) *total = 64u << 20;
        if (free_) *free_ = 64u << 20;
        return DD_OK;
    }
    virtual ~DirectDraw() {}
    ULONG refs = 1;
};

// ---- DirectSound -----------------------------------------------------------

#pragma pack(push, 1)
struct WAVEFORMATEX { WORD wFormatTag, nChannels; DWORD nSamplesPerSec, nAvgBytesPerSec; WORD nBlockAlign, wBitsPerSample, cbSize; };
#pragma pack(pop)
struct DSBUFFERDESC { DWORD dwSize, dwFlags, dwBufferBytes, dwReserved; WAVEFORMATEX *lpwfxFormat; };
struct DSBCAPS { DWORD dwSize, dwFlags, dwBufferBytes, dwUnlockTransferRate, dwPlayCpuOverhead; };

struct SoundBuffer;
std::mutex &mixer_mutex() { static std::mutex m; return m; }
bool g_mixer_active = false;
bool mixer_active() { return g_mixer_active; }
std::vector<SoundBuffer *> &mixer_buffers() { static std::vector<SoundBuffer *> v; return v; }
void attach(SoundBuffer *b);
void detach(SoundBuffer *b);

struct SoundBuffer {
    virtual HRESULT QueryInterface(const void *, void **o) { *o = this; AddRef(); return DD_OK; }
    virtual ULONG AddRef() { return ++refs; }
    virtual ULONG Release() { ULONG r = --refs; if (!r) delete this; return r; }
    virtual HRESULT GetCaps(DSBCAPS *c) { c->dwFlags = flags; c->dwBufferBytes = (DWORD)data.size(); return DD_OK; }
    virtual HRESULT GetCurrentPosition(DWORD *play, DWORD *write)
    {
        std::lock_guard<std::mutex> lock(mixer_mutex());
        DWORD p = position();
        if (play) *play = p;
        if (write) *write = data.empty() ? 0 : (DWORD)((p + format.nBlockAlign * 64u) % data.size());
        return DD_OK;
    }
    virtual HRESULT GetFormat(WAVEFORMATEX *f, DWORD size, DWORD *written)
    {
        if (f) memcpy(f, &format, size < sizeof format ? size : sizeof format);
        if (written) *written = sizeof format;
        return DD_OK;
    }
    virtual HRESULT GetVolume(LONG *v) { *v = volume; return DD_OK; }
    virtual HRESULT GetPan(LONG *p) { *p = pan; return DD_OK; }
    virtual HRESULT GetFrequency(DWORD *f) { *f = frequency; return DD_OK; }
    virtual HRESULT GetStatus(DWORD *s)
    {
        std::lock_guard<std::mutex> lock(mixer_mutex());
        if (playing && !looping && position_raw() >= data.size()) playing = false;
        *s = playing ? (1u | (looping ? 4u : 0u)) : 0u;
        return DD_OK;
    }
    virtual HRESULT Initialize(void *, const DSBUFFERDESC *) { return DD_OK; }
    virtual HRESULT Lock(DWORD offset, DWORD bytes, void **a1, DWORD *b1, void **a2, DWORD *b2, DWORD flags)
    {
        if (data.empty()) { *a1 = nullptr; *b1 = 0; if (a2) *a2 = nullptr; if (b2) *b2 = 0; return DD_OK; }
        if (flags & 1) offset = position();          // DSBLOCK_FROMWRITECURSOR
        if (flags & 2) bytes = (DWORD)data.size();   // DSBLOCK_ENTIREBUFFER
        offset %= (DWORD)data.size();
        DWORD first = bytes < data.size() - offset ? bytes : (DWORD)data.size() - offset;
        *a1 = data.data() + offset;
        *b1 = first;
        if (a2) *a2 = bytes > first ? data.data() : nullptr;
        if (b2) *b2 = bytes > first ? bytes - first : 0;
        return DD_OK;
    }
    virtual HRESULT Play(DWORD, DWORD, DWORD f)
    {
        std::lock_guard<std::mutex> lock(mixer_mutex());
        if (!mixer_active()) {
            static bool logged = false;
            if (!logged) { logged = true; unimplemented("DirectSound output (no host audio device: buffers play silently)"); }
            if (!playing) start = std::chrono::steady_clock::now() - std::chrono::microseconds(offset_us());
        } else if (!playing && cursor >= data.size()) {
            cursor = 0;
        }
        playing = true;
        looping = f & 1;
        return DD_OK;
    }
    virtual HRESULT SetCurrentPosition(DWORD p)
    {
        std::lock_guard<std::mutex> lock(mixer_mutex());
        cursor = p; frac = 0; start = std::chrono::steady_clock::now();
        return DD_OK;
    }
    virtual HRESULT SetFormat(const WAVEFORMATEX *f) { format = *f; frequency = f->nSamplesPerSec; return DD_OK; }
    virtual HRESULT SetVolume(LONG v) { volume = v; return DD_OK; }
    virtual HRESULT SetPan(LONG p) { pan = p; return DD_OK; }
    virtual HRESULT SetFrequency(DWORD f) { frequency = f ? f : format.nSamplesPerSec; return DD_OK; }
    virtual HRESULT Stop()
    {
        std::lock_guard<std::mutex> lock(mixer_mutex());
        cursor = position(); playing = false;
        return DD_OK;
    }
    virtual HRESULT Unlock(void *, DWORD, void *, DWORD) { return DD_OK; }
    virtual HRESULT Restore() { return DD_OK; }
    SoundBuffer() { attach(this); }
    SoundBuffer(const SoundBuffer &o) = default;
    virtual ~SoundBuffer() { detach(this); }

    ULONG refs = 1;
    DWORD flags = 0, frequency = 22050, cursor = 0;
    uint32_t frac = 0;                 // 16.16 fraction of a frame (mixer)
    LONG volume = 0, pan = 0;
    bool playing = false, looping = false;
    WAVEFORMATEX format = {1, 2, 22050, 88200, 4, 16, 0};
    std::vector<unsigned char> data;
    std::chrono::steady_clock::time_point start;

    uint64_t offset_us() const
    {
        DWORD rate = frequency * format.nBlockAlign;
        return rate ? (uint64_t)cursor * 1000000ull / rate : 0;
    }
    uint64_t position_raw() const
    {
        if (!playing || mixer_active()) return cursor;
        uint64_t us = (uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count();
        uint64_t bytes = us * frequency * format.nBlockAlign / 1000000ull;
        return format.nBlockAlign ? bytes / format.nBlockAlign * format.nBlockAlign : bytes;
    }
    DWORD position() const
    {
        if (data.empty()) return 0;
        if (mixer_active()) return cursor < data.size() ? cursor : (DWORD)data.size();
        uint64_t p = position_raw();
        return (DWORD)(looping ? p % data.size() : (p < data.size() ? p : data.size()));
    }
};

void attach(SoundBuffer *b)
{
    std::lock_guard<std::mutex> lock(mixer_mutex());
    mixer_buffers().push_back(b);
}
void detach(SoundBuffer *b)
{
    std::lock_guard<std::mutex> lock(mixer_mutex());
    std::vector<SoundBuffer *> &v = mixer_buffers();
    for (size_t i = 0; i < v.size(); ++i) if (v[i] == b) { v.erase(v.begin() + i); break; }
}

const int kMixRate = 44100;

// DirectSound attenuation (hundredths of a decibel, 0 .. -10000) as a gain.
float db_gain(LONG hundredths)
{
    if (hundredths >= 0) return 1.0f;
    if (hundredths <= -10000) return 0.0f;
    return powf(10.0f, (float)hundredths / 2000.0f);
}

// One sample of a buffer frame as a signed 16-bit value (channel 0 or 1).
inline int sample_at(const SoundBuffer *b, uint32_t frame, int channel)
{
    const unsigned char *p = b->data.data() + (size_t)frame * b->format.nBlockAlign;
    int ch = b->format.nChannels > 1 ? channel : 0;
    if (b->format.wBitsPerSample == 8) return ((int)p[ch] - 128) << 8;
    return (int)(short)(p[2 * ch] | p[2 * ch + 1] << 8);
}

// The host's audio thread: mixes every playing secondary buffer into out.
void mix(short *out, int frames)
{
    static std::vector<int> acc;
    acc.assign((size_t)frames * 2, 0);
    {
        std::lock_guard<std::mutex> lock(mixer_mutex());
        for (SoundBuffer *b : mixer_buffers()) {
            if (!b->playing || b->data.empty() || b->format.nBlockAlign == 0) continue;
            if (b->format.wBitsPerSample != 8 && b->format.wBitsPerSample != 16) continue;
            uint32_t total = (uint32_t)(b->data.size() / b->format.nBlockAlign);
            if (!total) continue;
            float volume = db_gain(b->volume);
            float left = volume * db_gain(b->pan > 0 ? -b->pan : 0);
            float right = volume * db_gain(b->pan < 0 ? b->pan : 0);
            int gl = (int)(left * 256.0f), gr = (int)(right * 256.0f);
            uint32_t step = (uint32_t)(((uint64_t)(b->frequency ? b->frequency : b->format.nSamplesPerSec) << 16) / kMixRate);
            uint32_t frame = b->cursor / b->format.nBlockAlign;
            uint32_t frac = b->frac;
            for (int i = 0; i < frames; ++i) {
                if (frame >= total) {
                    if (!b->looping) { b->playing = false; frame = total; break; }
                    frame %= total;
                }
                acc[2 * i] += sample_at(b, frame, 0) * gl >> 8;
                acc[2 * i + 1] += sample_at(b, frame, 1) * gr >> 8;
                frac += step;
                frame += frac >> 16;
                frac &= 0xFFFF;
            }
            if (b->looping && total) frame %= total;
            b->cursor = frame * b->format.nBlockAlign;
            b->frac = frac;
        }
    }
    for (int i = 0; i < frames * 2; ++i) {
        int v = acc[i];
        out[i] = (short)(v > 32767 ? 32767 : v < -32768 ? -32768 : v);
    }
}

void open_mixer()
{
    static bool tried = false;
    if (tried) return;
    tried = true;
    const Host *h = host();
    if (h && h->open_audio && h->open_audio(kMixRate, mix)) {
        std::lock_guard<std::mutex> lock(mixer_mutex());
        g_mixer_active = true;
    }
}

struct DirectSound {
    virtual HRESULT QueryInterface(const void *, void **o) { *o = this; AddRef(); return DD_OK; }
    virtual ULONG AddRef() { return ++refs; }
    virtual ULONG Release() { ULONG r = --refs; if (!r) delete this; return r; }
    virtual HRESULT CreateSoundBuffer(const DSBUFFERDESC *d, SoundBuffer **out, void *)
    {
        SoundBuffer *b = new SoundBuffer;
        b->flags = d->dwFlags;
        if (d->lpwfxFormat) { b->format = *d->lpwfxFormat; b->frequency = b->format.nSamplesPerSec; }
        if (!(d->dwFlags & 1)) b->data.assign(d->dwBufferBytes, 0);   // not DSBCAPS_PRIMARYBUFFER
        *out = b;
        return DD_OK;
    }
    virtual HRESULT GetCaps(void *caps)
    {
        DWORD size = *(DWORD *)caps;
        memset(caps, 0, size ? size : 96);
        *(DWORD *)caps = size;
        ((DWORD *)caps)[1] = 0x0F00;          // DSCAPS_*BITS/STEREO/MONO software
        ((DWORD *)caps)[2] = 100;             // min secondary rate
        ((DWORD *)caps)[3] = 100000;          // max secondary rate
        return DD_OK;
    }
    virtual HRESULT DuplicateSoundBuffer(SoundBuffer *o, SoundBuffer **d)
    {
        SoundBuffer *b;
        {
            std::lock_guard<std::mutex> lock(mixer_mutex());
            b = new SoundBuffer(*o);
            b->refs = 1;
            b->playing = false;
        }
        attach(b);
        *d = b;
        return DD_OK;
    }
    virtual HRESULT SetCooperativeLevel(HWND, DWORD) { return DD_OK; }
    virtual HRESULT Compact() { return DD_OK; }
    virtual HRESULT GetSpeakerConfig(DWORD *c) { *c = 4; return DD_OK; }   // DSSPEAKER_STEREO
    virtual HRESULT SetSpeakerConfig(DWORD) { return DD_OK; }
    virtual HRESULT Initialize(const void *) { return DD_OK; }
    virtual ~DirectSound() {}
    ULONG refs = 1;
};

}  // namespace

extern "C" {
HRESULT DirectDrawCreate(void *guid, DirectDraw **out, void *outer)
{
    (void)guid; (void)outer;
    progress("ddraw");
    *out = new DirectDraw;
    return DD_OK;
}
HRESULT DirectSoundCreate(const void *guid, DirectSound **out, void *outer)
{
    (void)guid; (void)outer;
    open_mixer();
    *out = new DirectSound;
    return DD_OK;
}
}
