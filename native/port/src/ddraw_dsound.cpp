// DirectDraw and DirectSound as C++ objects with the COM interface shapes the
// recovered code calls (shared/directdraw.hpp, shared/dsound_api.hpp: same slot
// order, same argument widths). Surfaces are system-memory pixel buffers; the
// primary surface is shown through the host on Flip/Blt/Unlock. Sound buffers
// keep their PCM and advance their play cursor by wall time; mixing them into
// an SDL audio stream is the next audio step (logged once).
#include "ra2port/win32_port.hpp"

#include <chrono>
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
            copy(d.left, d.top, src, s, (flags & DDBLT_KEYSRC) != 0);
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
    void present()
    {
        if (host() && host()->present) host()->present(pixels.data(), (int)width, (int)height, (int)pitch, (int)bytes * 8);
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

struct SoundBuffer {
    virtual HRESULT QueryInterface(const void *, void **o) { *o = this; AddRef(); return DD_OK; }
    virtual ULONG AddRef() { return ++refs; }
    virtual ULONG Release() { ULONG r = --refs; if (!r) delete this; return r; }
    virtual HRESULT GetCaps(DSBCAPS *c) { c->dwFlags = flags; c->dwBufferBytes = (DWORD)data.size(); return DD_OK; }
    virtual HRESULT GetCurrentPosition(DWORD *play, DWORD *write)
    {
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
        static bool logged = false;
        if (!logged) { logged = true; unimplemented("DirectSound mixing (buffers play silently)"); }
        if (!playing) start = std::chrono::steady_clock::now() - std::chrono::microseconds(offset_us());
        playing = true;
        looping = f & 1;
        return DD_OK;
    }
    virtual HRESULT SetCurrentPosition(DWORD p) { cursor = p; start = std::chrono::steady_clock::now(); return DD_OK; }
    virtual HRESULT SetFormat(const WAVEFORMATEX *f) { format = *f; frequency = f->nSamplesPerSec; return DD_OK; }
    virtual HRESULT SetVolume(LONG v) { volume = v; return DD_OK; }
    virtual HRESULT SetPan(LONG p) { pan = p; return DD_OK; }
    virtual HRESULT SetFrequency(DWORD f) { frequency = f ? f : format.nSamplesPerSec; return DD_OK; }
    virtual HRESULT Stop() { cursor = position(); playing = false; return DD_OK; }
    virtual HRESULT Unlock(void *, DWORD, void *, DWORD) { return DD_OK; }
    virtual HRESULT Restore() { return DD_OK; }
    virtual ~SoundBuffer() {}

    ULONG refs = 1;
    DWORD flags = 0, frequency = 22050, cursor = 0;
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
        if (!playing) return cursor;
        uint64_t us = (uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count();
        uint64_t bytes = us * frequency * format.nBlockAlign / 1000000ull;
        return format.nBlockAlign ? bytes / format.nBlockAlign * format.nBlockAlign : bytes;
    }
    DWORD position() const
    {
        if (data.empty()) return 0;
        uint64_t p = position_raw();
        return (DWORD)(looping ? p % data.size() : (p < data.size() ? p : data.size()));
    }
};

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
        SoundBuffer *b = new SoundBuffer(*o);
        b->refs = 1;
        b->playing = false;
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
    *out = new DirectDraw;
    return DD_OK;
}
HRESULT DirectSoundCreate(const void *guid, DirectSound **out, void *outer)
{
    (void)guid; (void)outer;
    *out = new DirectSound;
    return DD_OK;
}
}
