// binkw32 without a video decoder (Scout1133, native M16; timing Scout1138, M17).
// The game's movie code (BinkClass 0x0042FF30.., BinkMovieHandle 0x005A1A80,
// MSBinkAnim) runs as in the original: BinkOpen reads the movie's header (by
// name, or from the mix file handle at its current position under
// BINKFILEHANDLE 0x00800000) and returns a handle with the SDK's leading fields
// (shared/movie.hpp BinkHandleSize: Width, Height, Frames, FrameNum,
// LastFrameNum, FrameRate, FrameRateDiv); frames advance with
// BinkNextFrame/BinkGoto. M17: BinkWait answers from a clock, so frame n is
// due (n - 1) * FrameRateDiv / FrameRate seconds after the movie started (the
// header's rate; BinkPause stops the clock), and BinkCopyToBuffer writes a
// black frame (the decoder is not written yet: NOTES of Scout1138 hold the
// plan). Every movie (intro, briefings, radar transmissions) thus shows black
// for its own length, with no sound. RA2_NATIVE_MOVIE_SKIP=1 (test runs) keeps
// M16's pace: one frame per decode pass, so movies play out at once.
// BinkDDSurfaceType reports the port's DirectDraw format, RGB565 (BINKSURFACE565).
// A file that is not a Bink movie fails as BinkOpen does.
#include "ra2port/win32_port.hpp"

#include <chrono>
#include <stdlib.h>
#include <string.h>

using namespace ra2port;

namespace {
struct NativeBink {
    int Width;
    int Height;
    unsigned Frames;
    unsigned FrameNum;
    unsigned LastFrameNum;
    unsigned FrameRate;
    unsigned FrameRateDiv;
    unsigned reserved[16];   // the SDK handle is larger; nothing reads past +0x1C
    bool held;               // native (RA2_NATIVE_MOVIE_SKIP): the next frame is not due until BinkWait was asked once more
    double start;            // native: clock time of frame 1 (seconds)
    double paused_at;        // native: clock time BinkPause stopped it (0 while running)
};
const char *last_error = "";
unsigned le32(unsigned char const *p) { return p[0] | p[1] << 8 | p[2] << 16 | (unsigned)p[3] << 24; }
double now()
{
    return std::chrono::duration<double>(std::chrono::steady_clock::now().time_since_epoch()).count();
}
bool skip_movies()
{
    static int skip = -1;
    if (skip < 0) {
        const char *v = getenv("RA2_NATIVE_MOVIE_SKIP");
        skip = v && *v && *v != '0';
    }
    return skip != 0;
}
// Seconds from frame 1 to frame n at the header's rate.
double frame_time(NativeBink const *bink, unsigned n)
{
    return n > 1 ? (double)(n - 1) * bink->FrameRateDiv / bink->FrameRate : 0.0;
}
// Bytes per pixel of a BINKSURFACE type (flags' low bits): 8P, 24, 24R, 32, 32R,
// 32A, 32RA, 4444, 5551, 555, 565, 655, 664, YUY2, UYVY.
int surface_bytes(unsigned flags)
{
    unsigned type = flags & 0x0F;
    if (type == 0) return 1;
    if (type <= 2) return 3;
    if (type <= 6) return 4;
    if (type <= 14) return 2;
    return 0;
}
}

extern "C" {

// port/src/kernel32.cpp
HANDLE CreateFileA(const char *name, DWORD access, DWORD share, void *security, DWORD disposition, DWORD flags, HANDLE templ);
BOOL ReadFile(HANDLE handle, void *buffer, DWORD size, DWORD *read_bytes, void *overlapped);
BOOL CloseHandle(HANDLE handle);

void *BinkOpen(const char *name, DWORD flags)
{
    unsigned char header[44];
    DWORD got = 0;
    if (flags & 0x00800000u) {
        HANDLE file = (HANDLE)name;
        if (!ReadFile(file, header, sizeof header, &got, nullptr)) got = 0;
    } else {
        HANDLE file = CreateFileA(name, 0x80000000u, 1, nullptr, 3, 0, nullptr);
        if (file == RA2_INVALID_HANDLE) { last_error = "file not found"; return nullptr; }
        if (!ReadFile(file, header, sizeof header, &got, nullptr)) got = 0;
        CloseHandle(file);
    }
    if (got < sizeof header || header[0] != 'B' || header[1] != 'I' || header[2] != 'K') {
        last_error = "not a Bink file";
        trace("BinkOpen: not a Bink file");
        return nullptr;
    }
    NativeBink *bink = (NativeBink *)calloc(1, sizeof(NativeBink));
    bink->Frames = le32(header + 8);
    bink->Width = (int)le32(header + 20);
    bink->Height = (int)le32(header + 24);
    bink->FrameRate = le32(header + 28);
    bink->FrameRateDiv = le32(header + 32);
    if (bink->FrameRateDiv == 0) bink->FrameRateDiv = 1;
    if (bink->FrameRate < bink->FrameRateDiv) bink->FrameRate = bink->FrameRateDiv;
    bink->FrameNum = 1;
    bink->start = now();
    trace("BinkOpen: %dx%d, %u frames at %u/%u fps (black frames, no decoder)", bink->Width, bink->Height,
          bink->Frames, bink->FrameRate, bink->FrameRateDiv);
    return bink;
}
void BinkClose(void *bink) { free(bink); }
// A black frame at x, y of the destination (no decoder yet), clipped to its height.
int BinkCopyToBuffer(void *handle, void *dest, int pitch, unsigned height, unsigned x, unsigned y, unsigned flags)
{
    NativeBink *bink = (NativeBink *)handle;
    int bytes = surface_bytes(flags);
    if (!bink || !dest || pitch <= 0 || bytes == 0) return 0;
    for (unsigned row = 0; row < (unsigned)bink->Height && y + row < height; ++row) {
        memset((unsigned char *)dest + (size_t)(y + row) * (unsigned)pitch + (size_t)x * bytes, 0,
               (size_t)bink->Width * bytes);
    }
    return 0;
}
int BinkDDSurfaceType(void *) { return 10; }   // BINKSURFACE565: the port's DirectDraw surfaces
int BinkDoFrame(void *) { return 0; }
const char *BinkGetError(void) { return last_error; }
int BinkGoto(void *handle, unsigned frame, int)
{
    NativeBink *bink = (NativeBink *)handle;
    if (bink) {
        bink->LastFrameNum = bink->FrameNum;
        bink->FrameNum = frame ? frame : 1;
        bink->start = now() - frame_time(bink, bink->FrameNum);
        if (bink->paused_at) bink->paused_at = now();
    }
    return 0;
}
void BinkNextFrame(void *handle)
{
    NativeBink *bink = (NativeBink *)handle;
    if (bink) { bink->LastFrameNum = bink->FrameNum; ++bink->FrameNum; bink->held = true; }
}
int BinkOpenDirectSound(uintptr_t) { return 0; }
int BinkSetSoundSystem(void *, uintptr_t) { return 0; }
// Stops (nonzero) or restarts the movie's clock; returns the paused state.
int BinkPause(void *handle, int pause)
{
    NativeBink *bink = (NativeBink *)handle;
    if (!bink) return 0;
    if (pause && !bink->paused_at) {
        bink->paused_at = now();
    } else if (!pause && bink->paused_at) {
        bink->start += now() - bink->paused_at;
        bink->paused_at = 0;
    }
    return bink->paused_at != 0;
}
void BinkSetVolume(void *, int) {}
// Nonzero while the next frame is not due: frame FrameNum is due frame_time
// after the start (paused movies wait). Under RA2_NATIVE_MOVIE_SKIP each frame
// is due on the BinkWait after the one that follows BinkNextFrame: the decode
// loops (BinkClass 0x00430600: `do { ... BinkNextFrame } while (BinkWait() == 0)`)
// show one frame per pass, as fast as the caller loops.
int BinkWait(void *handle)
{
    NativeBink *bink = (NativeBink *)handle;
    if (!bink) return 0;
    if (skip_movies()) {
        if (bink->held) { bink->held = false; return 1; }
        return 0;
    }
    if (bink->paused_at) return 1;
    return now() - bink->start < frame_time(bink, bink->FrameNum) ? 1 : 0;
}

}  // extern "C"
