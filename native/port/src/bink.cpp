// binkw32 without a video decoder (Scout1133, native M16). The game's movie
// code (BinkClass 0x0042FF30.., BinkMovieHandle 0x005A1A80, MSBinkAnim) runs as
// in the original: BinkOpen reads the movie's header (by name, or from the
// mix file handle at its current position under BINKFILEHANDLE 0x00800000)
// and returns a handle with the SDK's leading fields (shared/movie.hpp
// BinkHandleSize: Width, Height, Frames, FrameNum, LastFrameNum, FrameRate,
// FrameRateDiv); frames advance with BinkNextFrame/BinkGoto, BinkWait holds
// back only the call right after a frame (one frame per decode pass), and no pixels are decoded (BinkCopyToBuffer leaves the
// target as it is). So every movie plays out at once with no picture or
// sound: the "skip with no video" the native lane takes until a decoder
// decision. A file that is not a Bink movie fails as BinkOpen does.
#include "ra2port/win32_port.hpp"

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
    bool held;               // native: the next frame is not due until BinkWait was asked once more
};
const char *last_error = "";
unsigned le32(unsigned char const *p) { return p[0] | p[1] << 8 | p[2] << 16 | (unsigned)p[3] << 24; }
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
    trace("BinkOpen: %dx%d, %u frames (no video)", bink->Width, bink->Height, bink->Frames);
    return bink;
}
void BinkClose(void *bink) { free(bink); }
int BinkCopyToBuffer(void *, void *, int, unsigned, unsigned, unsigned, unsigned) { return 0; }
int BinkDDSurfaceType(void *) { return 0; }
int BinkDoFrame(void *) { return 0; }
const char *BinkGetError(void) { return last_error; }
int BinkGoto(void *handle, unsigned frame, int)
{
    NativeBink *bink = (NativeBink *)handle;
    if (bink) { bink->LastFrameNum = bink->FrameNum; bink->FrameNum = frame ? frame : 1; }
    return 0;
}
void BinkNextFrame(void *handle)
{
    NativeBink *bink = (NativeBink *)handle;
    if (bink) { bink->LastFrameNum = bink->FrameNum; ++bink->FrameNum; bink->held = true; }
}
int BinkOpenDirectSound(uintptr_t) { return 0; }
int BinkSetSoundSystem(void *, uintptr_t) { return 0; }
void BinkPause(void *, int) {}
void BinkSetVolume(void *, int) {}
// Nonzero while the next frame is not due. Without a clock each frame is due
// on the BinkWait after the one that follows BinkNextFrame: the decode loops
// (BinkClass 0x00430600: `do { ... BinkNextFrame } while (BinkWait() == 0)`)
// show one frame per pass, as fast as the caller loops.
int BinkWait(void *handle)
{
    NativeBink *bink = (NativeBink *)handle;
    if (bink && bink->held) { bink->held = false; return 1; }
    return 0;
}

}  // extern "C"
