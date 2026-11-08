// Imports the native game does not need yet: logged no-ops returning the
// failure value the recovered callers already handle (NULL handle, FALSE, 0,
// E_NOTIMPL). Each logs once (ra2port::unimplemented). Listed in native/README.md.
#include "ra2port/win32_port.hpp"

using namespace ra2port;

extern "C" {

// SHELL32: opening the browser/readme.
HINSTANCE FindExecutableA(const char *, const char *, char *result) { unimplemented("FindExecutableA"); if (result) *result = 0; return (HINSTANCE)(uintptr_t)2; }

// VERSION: version resources of other files (DirectX/driver checks).
DWORD GetFileVersionInfoSizeA(const char *, DWORD *handle) { unimplemented("GetFileVersionInfoSizeA"); if (handle) *handle = 0; return 0; }
BOOL GetFileVersionInfoA(const char *, DWORD, DWORD, void *) { unimplemented("GetFileVersionInfoA"); return 0; }
BOOL VerQueryValueA(const void *, const char *, void **out, UINT *size) { unimplemented("VerQueryValueA"); if (out) *out = nullptr; if (size) *size = 0; return 0; }

// COMCTL32: drag images in the WOL/lobby list controls.
void InitCommonControls(void) {}
BOOL ImageList_BeginDrag(void *, int, int, int) { unimplemented("ImageList_BeginDrag"); return 0; }
BOOL ImageList_Destroy(void *) { return 0; }
BOOL ImageList_DragEnter(HWND, int, int) { return 0; }
BOOL ImageList_DragMove(int, int) { return 0; }
BOOL ImageList_DragShowNolock(BOOL) { return 0; }
void ImageList_EndDrag(void) {}

// IMM32: East Asian input methods (no IME on the native host yet).
void *ImmGetContext(HWND) { return nullptr; }
LONG ImmGetCompositionStringA(void *, DWORD, void *, DWORD) { return 0; }
DWORD ImmGetCandidateListA(void *, DWORD, void *, DWORD) { return 0; }
BOOL ImmNotifyIME(void *, DWORD, DWORD, DWORD) { return 0; }
BOOL ImmSetOpenStatus(void *, BOOL) { return 0; }

// binkw32: Bink movies (the intro/briefing videos) need a decoder decision;
// BinkOpen fails, so the game skips each movie.
void *BinkOpen(const char *name, DWORD) { trace("BinkOpen %s", name ? name : ""); unimplemented("BinkOpen (Bink movies)"); return nullptr; }
void BinkClose(void *) {}
int BinkCopyToBuffer(void *, void *, int, unsigned, unsigned, unsigned, unsigned) { return 0; }
int BinkDDSurfaceType(void *) { return 0; }
int BinkDoFrame(void *) { return 0; }
const char *BinkGetError(void) { return "Bink is not available in the native build"; }
int BinkGoto(void *, unsigned, int) { return 0; }
void BinkNextFrame(void *) {}
int BinkOpenDirectSound(uintptr_t) { return 0; }
int BinkSetSoundSystem(void *, uintptr_t) { return 0; }
void BinkPause(void *, int) {}
void BinkSetVolume(void *, int) {}
int BinkWait(void *) { return 0; }

}  // extern "C"
