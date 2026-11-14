// dsurface.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/dsurface.json carries the addresses and the rows that
// stay in their own files.


#define RA2_DSURFACE_DRAW_TRAIL_LINE  // DSurface::Draw_Trail_Line_4AE9C0 0x004AE9C0 is defined in this unit
// Not exact (RA2_NON_MATCHING) prelude: header switches the drafts below need (guard-on compile only)
#ifdef RA2_NON_MATCHING
#define RGB_DECLARE_ACCESSORS
#endif

#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/crt_memory.hpp"
#include "shared/dsurface.hpp"
#include "shared/hicolor.hpp"
#include "shared/win32_api.hpp"
#include "shared/rect_intersect.hpp"
#include "shared/rgb_class.hpp"
#include "shared/palette.hpp"
#include "shared/always.hpp"
#include "shared/blit.hpp"
#include "shared/fast_math.hpp"
#include "shared/direct3d.hpp"


// Header-static dynamic initializers of the unit at 0x004AA2B0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x004AA2B0-0x004AA491); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x004AA2D0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_004aa2b0.cpp; no source of its own.


// Dynamic initializer (0x004AA300, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_004aa2b0.cpp; no source of its own.


// Dynamic initializer (0x004AA320, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_004aa2b0.cpp; no source of its own.


// Dynamic initializer (0x004AA340, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_004aa2b0.cpp; no source of its own.


// Dynamic initializer (0x004AA360, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_004aa2b0.cpp; no source of its own.


// Dynamic initializer (0x004AA380, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_004aa2b0.cpp; no source of its own.


// Dynamic initializer (0x004AA3A0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_004aa2b0.cpp; no source of its own.


// Dynamic initializer (0x004AA3E0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_004aa2b0.cpp; no source of its own.


// Dynamic initializer (0x004AA410, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_004aa2b0.cpp; no source of its own.


// Dynamic initializer (0x004AA440, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_004aa2b0.cpp; no source of its own.


// Dynamic initializer (0x004AA470, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_004aa2b0.cpp; no source of its own.


// Dynamic initializer (0x004AA480, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_004aa2b0.cpp; no source of its own.


// DSurface::DSurface(int width, int height, unsigned char system_memory) (0x004AA4A0)
// Creates an offscreen DirectDraw surface of the given size (system memory
// when asked; a plain offscreen 3D-device surface when unknown_flag_852fd7
// is set) and reads back its
// description, pixel size and memory placement.
// Unresolved: the meaning of the global flag; CreateSurface's result is not
// checked (only the returned pointer).

DSurface::DSurface(int width, int height, unsigned char system_memory)
    : XSurface(width, height),
      BytesPerPixel(0),
      BufferPtr(0),
      IsPrimary(false),
      IsVideoRam(false),
      VideoSurfacePtr(0),
      VideoSurfaceDescription(0)
{
    VideoSurfaceDescription = new DDSURFACEDESC;
    if (VideoSurfaceDescription != 0) {
        memset(VideoSurfaceDescription, 0, sizeof(DDSURFACEDESC));
        VideoSurfaceDescription->dwSize = sizeof(DDSURFACEDESC);
        VideoSurfaceDescription->dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
        VideoSurfaceDescription->dwWidth = width;
        VideoSurfaceDescription->dwHeight = height;
        if (system_memory == true) {
            VideoSurfaceDescription->ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
        }
        if (unknown_flag_852fd7) {
            VideoSurfaceDescription->ddsCaps.dwCaps = DDSCAPS_3DDEVICE | DDSCAPS_OFFSCREENPLAIN;
        }
        DirectDrawObject->CreateSurface(VideoSurfaceDescription, &VideoSurfacePtr, 0);
        if (VideoSurfacePtr != 0) {
            memset(VideoSurfaceDescription, 0, sizeof(DDSURFACEDESC));
            VideoSurfaceDescription->dwSize = sizeof(DDSURFACEDESC);
            VideoSurfacePtr->GetSurfaceDesc(VideoSurfaceDescription);
            BytesPerPixel = (VideoSurfaceDescription->ddpfPixelFormat.dwRGBBitCount + 7) / 8;
            IsVideoRam = (VideoSurfaceDescription->ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY) != 0;
        }
    }
}


// DSurface::~DSurface() (0x004AA5A0)
// Releases the primary surface's clipper (primary only), the description
// and the DirectDraw surface.

DSurface::~DSurface()
{
    if (IsPrimary && VideoSurfacePtr != 0 && unknown_clipper_852fb4 != 0) {
        VideoSurfacePtr->SetClipper(0);
        unknown_clipper_852fb4->Release();
        unknown_clipper_852fb4 = 0;
    }
    delete VideoSurfaceDescription;
    VideoSurfaceDescription = 0;
    if (VideoSurfacePtr != 0) {
        VideoSurfacePtr->Release();
    }
    VideoSurfacePtr = 0;
}


// DSurface::DSurface() (0x004AA610)
// An empty surface with a cleared description (no DirectDraw surface yet).
// Unresolved: the allocation is used without a null check (as the original).

DSurface::DSurface()
    : BytesPerPixel(0),
      BufferPtr(0),
      IsPrimary(false),
      IsVideoRam(false),
      VideoSurfacePtr(0)
{
    VideoSurfaceDescription = new DDSURFACEDESC;
    memset(VideoSurfaceDescription, 0, sizeof(DDSURFACEDESC));
    VideoSurfaceDescription->dwSize = sizeof(DDSURFACEDESC);
}


// Creates the primary DirectDraw surface and optionally returns its back buffer.
// Full frozen baseline bound: 004AA660..004AAB5D, ret at 004AAB5D.
WIN32_IMPORT HWND WINAPI GetActiveWindow();
void __cdecl Debug_Print_004068f0(char const *format, ...);
unsigned int Get_Device_Caps_00495fc0();
void __fastcall unknown_00495770(HRESULT result, int show_success);
extern IDirectDrawSurface *PrimarySurface_8523d8;
extern DDPIXELFORMAT PrimaryPixelFormat_852b38;
extern unsigned short unknown_eighth_mask_852fd4;
extern char const CreatePrimary[]; // @ref ?CreatePrimary@@3QBDB=007d63b0
extern char const AllowStretch[]; // @ref ?AllowStretch@@3QBDB=007d638c
extern char const AllowHWFill[]; // @ref ?AllowHWFill@@3QBDB=007d6370
extern char const CreateSurface[]; // @ref ?CreateSurface@@3QBDB=007d6340
extern char const CreateFailed[]; // @ref ?CreateFailed@@3QBDB=007d62b0
extern char const CreateOK[]; // @ref ?CreateOK@@3QBDB=007d632c
extern char const GetDescFailed[]; // @ref ?GetDescFailed@@3QBDB=007d62fc
extern char const CreateDone[]; // @ref ?CreateDone@@3QBDB=007d62dc
extern char const TrueText[]; // @ref ?TrueText@@3QBDB=007d24d8
extern char const FalseText[]; // @ref ?FalseText@@3QBDB=007d24d0
inline bool Match_Primary_Format(int red_left, int red_right, int green_left, int green_right, int blue_left, int blue_right) {
    return unknown_shift_852fc0 == blue_left && unknown_shift_852fc4 == blue_right && unknown_shift_852fc8 == green_left && unknown_shift_852fcc == green_right && unknown_shift_852fb8 == red_left && unknown_shift_852fbc == red_right;
}
// Recognize the observed channel layouts; preserve the previous mode for an unknown layout.
inline void Select_Primary_Format(int red_left, int red_right, int green_left, int green_right, int blue_left, int blue_right) {
    if (blue_left==0 && blue_right==3 && green_left==5 && green_right==3 && red_left==10 && red_right==3) { unknown_7d6290=0; return; }
    if (blue_left==0 && blue_right==2 && green_left==6 && green_right==3 && red_left==11 && red_right==3) { unknown_7d6290=1; return; }
    if (blue_left==0 && blue_right==3 && green_left==5 && green_right==2 && red_left==11 && red_right==3) { unknown_7d6290=2; return; }
    if (blue_left==0 && blue_right==3 && green_left==5 && green_right==3 && red_left==11 && red_right==2) { unknown_7d6290=1; return; }
}
DSurface *__fastcall DSurface::Create_Primary(DSurface **back_buffer)
{
    Debug_Print_004068f0(CreatePrimary);
    unsigned int caps = Get_Device_Caps_00495fc0();
    AllowStretchBlits_852fd6 = false;
    if (caps & 0x40) AllowStretchBlits_852fd6 = true;
    Debug_Print_004068f0(AllowStretch, AllowStretchBlits_852fd6 ? TrueText : FalseText);
    if (!(caps & 0x10)) unknown_flag_7d6294 = false;
    Debug_Print_004068f0(AllowHWFill, unknown_flag_7d6294 == true ? TrueText : FalseText);
    DSurface *surface = new DSurface;
    int count = back_buffer != 0;
    surface->VideoSurfaceDescription->dwFlags = 1;
    if (unknown_flag_852fd7)
        surface->VideoSurfaceDescription->ddsCaps.dwCaps = 0x2200;
    else
        surface->VideoSurfaceDescription->ddsCaps.dwCaps = 0x200;
    if (count > 0) {
        surface->VideoSurfaceDescription->ddsCaps.dwCaps |= 0x18;
        surface->VideoSurfaceDescription->dwFlags |= 0x20;
        surface->VideoSurfaceDescription->dwBackBufferCount = count;
    }
    Debug_Print_004068f0(CreateSurface);
    HRESULT result = DirectDrawObject->CreateSurface(surface->VideoSurfaceDescription, &surface->VideoSurfacePtr, 0);
    if (result == DD_OK) {
    Debug_Print_004068f0(CreateOK);
    if (count > 0) {
        DDSCAPS attached_caps;
        attached_caps.dwCaps = 4;
        IDirectDrawSurface *attached;
        if (surface->VideoSurfacePtr->GetAttachedSurface(&attached_caps, &attached) == DD_OK)
            *back_buffer = new DSurface(attached);
    }
    memset(surface->VideoSurfaceDescription, 0, sizeof(DDSURFACEDESC));
    surface->VideoSurfaceDescription->dwSize = sizeof(DDSURFACEDESC);
    if (surface->VideoSurfacePtr->GetSurfaceDesc(surface->VideoSurfaceDescription) != DD_OK)
        Debug_Print_004068f0(GetDescFailed);
    surface->BytesPerPixel = (surface->VideoSurfaceDescription->ddpfPixelFormat.dwRGBBitCount + 7U) >> 3;
    surface->IsVideoRam = (surface->VideoSurfaceDescription->ddsCaps.dwCaps >> 14) & 1;
    surface->IsPrimary = true;
    surface->Width = surface->VideoSurfaceDescription->dwWidth;
    surface->Height = surface->VideoSurfaceDescription->dwHeight;
    PrimarySurface_8523d8 = surface->VideoSurfacePtr;
    if (DirectDrawObject->CreateClipper(0, &unknown_clipper_852fb4, 0) == DD_OK) {
        if (unknown_clipper_852fb4->SetHWnd(0, GetActiveWindow()) == DD_OK)
            surface->VideoSurfacePtr->SetClipper(unknown_clipper_852fb4);
    }
    PrimaryPixelFormat_852b38 = surface->VideoSurfaceDescription->ddpfPixelFormat;
    if (surface->Bytes_Per_Pixel() == 2) {
        unknown_shift_852fb8 = 0;
        unknown_shift_852fbc = 0;
        unsigned int red_mask = PrimaryPixelFormat_852b38.dwRBitMask;
        while (unknown_shift_852fb8 < 16) {
            if (red_mask & 1) break;
            red_mask = (int)red_mask >> 1;
            ++unknown_shift_852fb8;
        }
        while (unknown_shift_852fbc < 8) {
            if (red_mask & 0x80) break;
            red_mask <<= 1;
            ++unknown_shift_852fbc;
        }
        unknown_shift_852fc8 = 0;
        unknown_shift_852fcc = 0;
        unsigned int green_mask = PrimaryPixelFormat_852b38.dwGBitMask;
        while (unknown_shift_852fc8 < 16) {
            if (green_mask & 1) break;
            ++unknown_shift_852fc8;
            green_mask = (int)green_mask >> 1;
        }
        while (unknown_shift_852fcc < 8) {
            if (green_mask & 0x80) break;
            ++unknown_shift_852fcc;
            green_mask <<= 1;
        }
        unknown_shift_852fc0 = 0;
        unknown_shift_852fc4 = 0;
        unsigned int blue_mask = PrimaryPixelFormat_852b38.dwBBitMask;
        while (unknown_shift_852fc0 < 16) {
            if (blue_mask & 1) break;
            ++unknown_shift_852fc0;
            blue_mask = (int)blue_mask >> 1;
        }
        while (unknown_shift_852fc4 < 8) {
            if (blue_mask & 0x80) break;
            ++unknown_shift_852fc4;
            blue_mask <<= 1;
        }
        unknown_7d6290 = -1;
        unknown_half_mask_852fd0 =
            (unsigned short)((127 >> unknown_shift_852fc4) << unknown_shift_852fc0) |
            (unsigned short)((127 >> unknown_shift_852fcc) << unknown_shift_852fc8) |
            (unsigned short)((127 >> unknown_shift_852fbc) << unknown_shift_852fb8);
        unknown_quarter_mask_852fd2 =
            (unsigned short)((63 >> unknown_shift_852fc4) << unknown_shift_852fc0) |
            (unsigned short)((63 >> unknown_shift_852fcc) << unknown_shift_852fc8) |
            (unsigned short)((63 >> unknown_shift_852fbc) << unknown_shift_852fb8);
        unknown_eighth_mask_852fd4 =
            (unsigned short)((31 >> unknown_shift_852fc4) << unknown_shift_852fc0) |
            (unsigned short)((31 >> unknown_shift_852fcc) << unknown_shift_852fc8) |
            (unsigned short)((31 >> unknown_shift_852fbc) << unknown_shift_852fb8);
        Select_Primary_Format(unknown_shift_852fb8, unknown_shift_852fbc, unknown_shift_852fc8, unknown_shift_852fcc, unknown_shift_852fc0, unknown_shift_852fc4);
    }
done:
    Debug_Print_004068f0(CreateDone);
    return surface;
    }
    unknown_00495770(result, 1);
    Debug_Print_004068f0(CreateFailed, result);
    delete surface;
    return 0;
}


// DSurface::DSurface(IDirectDrawSurface *surface) (0x004AAB60)
// Wraps an existing DirectDraw surface and takes its size, pixel size and
// memory placement from its description.

DSurface::DSurface(IDirectDrawSurface *surface)
    : BytesPerPixel(0),
      BufferPtr(0),
      IsPrimary(false),
      IsVideoRam(false),
      VideoSurfacePtr(surface),
      VideoSurfaceDescription(0)
{
    if (surface != 0) {
        VideoSurfaceDescription = new DDSURFACEDESC;
        memset(VideoSurfaceDescription, 0, sizeof(DDSURFACEDESC));
        VideoSurfaceDescription->dwSize = sizeof(DDSURFACEDESC);
        if (VideoSurfacePtr->GetSurfaceDesc(VideoSurfaceDescription) == DD_OK) {
            BytesPerPixel = (VideoSurfaceDescription->ddpfPixelFormat.dwRGBBitCount + 7) / 8;
            IsVideoRam = (VideoSurfaceDescription->ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY) != 0;
            Width = VideoSurfaceDescription->dwWidth;
            Height = VideoSurfaceDescription->dwHeight;
        }
    }
}


// HDC DSurface::Get_DC() (0x004AABF0)
// Device context of the DirectDraw surface; holding it counts as a lock.
// Null when the surface is already locked or GetDC fails.

HDC DSurface::Get_DC()
{
    HDC dc = 0;
    if (!Is_Locked()) {
        if (VideoSurfacePtr->GetDC(&dc) == DD_OK) {
            LockLevel++;
        } else {
            return 0;
        }
    }
    return dc;
}


// int DSurface::Release_DC(HDC dc) (0x004AAC30)
// Gives a device context from Get_DC back and drops its lock.

int DSurface::Release_DC(HDC dc)
{
    if (VideoSurfacePtr->ReleaseDC(dc) == DD_OK && LockLevel > 0) {
        LockLevel--;
    }
    return 1;
}


// int DSurface::Bytes_Per_Pixel() (0x004AAC60, slot 28)
// Pixel size from the surface's pixel format.

int DSurface::Bytes_Per_Pixel()
{
    return BytesPerPixel;
}


// int DSurface::Stride() (0x004AAC70, slot 29)
// Row pitch reported by DirectDraw.

int DSurface::Stride()
{
    return VideoSurfaceDescription->lPitch;
}


// void *DSurface::Lock(Point2D point) (0x004AAC80, slot 23)
// Locks the DirectDraw surface on the first nested lock (refreshing the
// description, pixel size and memory pointer) and returns the address of
// pixel point; null when restoration fails or is disabled, or for a negative coordinate.

// Restores a lost DirectDraw surface and re-establishes an outstanding lock:
// the body of Restore_Check (0x004AAF00), which Lock, Unlock and Fill_Rect
// expand inline while Restore_Check itself is an ordinary function.
inline bool DSurface::Restore_Lost_Surface()
{
    if (unknown_flag_851cb0 || unknown_flag_a40d28) {
        if (VideoSurfacePtr != 0 && VideoSurfacePtr->IsLost() == DDERR_SURFACELOST) {
            if (VideoSurfacePtr->Restore() != DD_OK || VideoSurfacePtr->IsLost() != DD_OK) {
                return false;
            }
            if (LockLevel > 0) {
                int oldlocklevel = LockLevel;
                LockLevel = 0;
                Lock();
                LockLevel++;
                Unlock();
                LockLevel = oldlocklevel;
            }
        }
        return true;
    }
    return false;
}

void *DSurface::Lock(Point2D point)
{
    if (!Restore_Lost_Surface()) {
        return 0;
    }
    if (point.X < 0 || point.Y < 0) {
        return 0;
    }
    if (LockLevel == 0) {
        DDSURFACEDESC desc;
        memset(&desc, 0, sizeof(desc));
        desc.dwSize = sizeof(desc);
        if (VideoSurfacePtr->Lock(0, &desc, DDLOCK_WAIT, 0) != DD_OK) {
            return 0;
        }
        *VideoSurfaceDescription = desc;
        BytesPerPixel = (VideoSurfaceDescription->ddpfPixelFormat.dwRGBBitCount + 7) / 8;
        IsVideoRam = (VideoSurfaceDescription->ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY) != 0;
        BufferPtr = VideoSurfaceDescription->lpSurface;
    }
    XSurface::Lock();
    return (char *)BufferPtr + point.Y * Stride() + point.X * Bytes_Per_Pixel();
}


// bool DSurface::Can_Lock(int x, int y) (0x004AADC0, slot 25)
// When not locked, tries a DirectDraw lock and releases it at once; false
// when the lock fails. x and y are ignored.

bool DSurface::Can_Lock(int x, int y)
{
    if (LockLevel == 0) {
        DDSURFACEDESC desc;
        memset(&desc, 0, sizeof(desc));
        desc.dwSize = sizeof(desc);
        if (VideoSurfacePtr->Lock(0, &desc, 0, 0) != DD_OK) {
            return false;
        }
        VideoSurfacePtr->Unlock(desc.lpSurface);
    }
    return true;
}


// bool DSurface::Can_Blit() (0x004AAE20, slot 37)
// Whether DirectDraw can accept a blit now (GetBltStatus(DDGBS_CANBLT)).

bool DSurface::Can_Blit()
{
    return VideoSurfacePtr->GetBltStatus(DDGBS_CANBLT) == DD_OK;
}


// bool DSurface::Unlock() (0x004AAE40, slot 24)
// Drops one lock; the last one unlocks the DirectDraw surface. Restoration
// is attempted first but its result is ignored.

bool DSurface::Unlock()
{
    Restore_Lost_Surface();
    if (LockLevel > 0) {
        LockLevel--;
        if (LockLevel == 0) {
            VideoSurfacePtr->Unlock(BufferPtr);
            BufferPtr = 0;
        }
        return true;
    }
    return false;
}


// bool DSurface::Is_Surface_Lost() (0x004AAEE0)
// The DirectDraw surface exists and reports DDERR_SURFACELOST.

bool DSurface::Is_Surface_Lost()
{
    return VideoSurfacePtr != 0 && VideoSurfacePtr->IsLost() == DDERR_SURFACELOST;
}


// bool DSurface::Restore_Check() (0x004AAF00)
// Restores a lost DirectDraw surface and, when a lock is outstanding, locks
// it again to refresh the memory pointer. False when restoration is
// disabled (both flags clear) or Restore fails. The body is the inline
// Restore_Lost_Surface (defined with Lock), which Lock, Unlock and
// Fill_Rect expand. Unresolved: the meaning of the two enabling flags.

bool DSurface::Restore_Check()
{
    return Restore_Lost_Surface();
}


// bool DSurface::Copy_From(Rect const &toarea, Surface &source, Rect const &fromarea, bool trans, bool unknown) (0x004AAF80, slot 2)
// Copies fromarea of source to toarea, both clipped to their surfaces.

bool DSurface::Copy_From(Rect const &toarea, Surface &source, Rect const &fromarea, bool trans, bool unknown)
{
    return Copy_From(Get_Rect(), toarea, source, source.Get_Rect(), fromarea, trans, unknown);
}


// Copy fromarea of source (inside fromclip) to toarea of this surface (inside
// toclip) with a DirectDraw blit when both are DirectDraw surfaces of the same
// pixel size; the software copy handles transparent, locked or mismatched
// sources and same-size copies that DirectDraw would do in system memory.

bool DSurface::Copy_From(Rect const &toclip, Rect const &toarea, Surface &source,
                         Rect const &fromclip, Rect const &fromarea, bool trans, bool unknown)
{
    if (toclip.Width <= 0 || toclip.Height <= 0 || fromclip.Width <= 0 || fromclip.Height <= 0
        || toarea.Width <= 0 || toarea.Height <= 0 || fromarea.Width <= 0 || fromarea.Height <= 0) {
        return false;
    }
    bool software = false;
    if (!source.Is_Direct_Draw() || source.Is_Locked() == true || trans == true
        || Bytes_Per_Pixel() != source.Bytes_Per_Pixel()) {
        software = true;
    }
    bool system_copy = (IsPrimary == true && unknown_flag_851cb0 == true) ? false : unknown;
    if (!software && system_copy == true) {
        if (!IsVideoRam || (source.Is_Direct_Draw() == true && !((DSurface &)source).IsVideoRam)) {
            if (fromarea.Width == toarea.Width && fromarea.Height == toarea.Height) {
                software = true;
            }
        }
    }
    if (!Restore_Lost_Surface()) {
        return false;
    }
    if (software == true) {
        return XSurface::Copy_From(toarea, source, fromarea, trans, true);
    }
    Rect destination_area = toarea;
    Rect source_area = fromarea;
    Rect source_window = fromclip.Intersect(source.Get_Rect());
    Rect destination_window = toclip.Intersect(Get_Rect());
    if (Clip_Blit_Rects(destination_area, destination_window, source_area, source_window)) {
        RECT destination_rectangle;
        destination_rectangle.left = destination_area.X + destination_window.X;
        destination_rectangle.top = destination_area.Y + destination_window.Y;
        destination_rectangle.right = destination_area.X + destination_window.X + destination_area.Width;
        destination_rectangle.bottom = destination_area.Y + destination_window.Y + destination_area.Height;
        RECT source_rectangle;
        source_rectangle.left = source_area.X + source_window.X;
        source_rectangle.top = source_area.Y + source_window.Y;
        source_rectangle.right = source_area.X + source_window.X + source_area.Width;
        source_rectangle.bottom = source_area.Y + source_window.Y + source_area.Height;
        return VideoSurfacePtr->Blt(&destination_rectangle, ((DSurface &)source).VideoSurfacePtr,
                                    &source_rectangle, DDBLT_WAIT, 0) == DD_OK;
    }
    return false;
}


// bool DSurface::Fill_Rect(Rect const &fillrect, int color) (0x004AB4F0, slot 5)
// Fills fillrect clipped to the whole surface.

bool DSurface::Fill_Rect(Rect const &fillrect, int color)
{
    return DSurface::Fill_Rect(Get_Rect(), fillrect, color);
}


// Fill through DirectDraw when available and idle, otherwise use the software surface fill.

inline Rect Offset_Fill_Rect(Rect const &window, Rect const &area) {
    Point2D origin(window.X, window.Y);
    Point2D offset(area.X, area.Y);
    Point2D position(origin.X + offset.X, offset.Y + origin.Y);
    return Rect(position.X, position.Y, area.Width, area.Height);
}
bool DSurface::Fill_Rect(Rect const &cliprect, Rect const &fillrect, int color)
{
    RECT rectangle;
    if (VideoSurfacePtr != 0 && fillrect.Is_Valid()) {
        if (unknown_flag_7d6294 && !Is_Locked() && VideoSurfacePtr->GetBltStatus(DDGBS_CANBLT) == DD_OK) {
            if (!Restore_Lost_Surface()) return false;
            Rect clipped = cliprect.Intersect(Get_Rect());
            Rect area = Offset_Fill_Rect(cliprect, fillrect);
            area = area.Intersect(clipped);
            if (area.Is_Valid()) {
                rectangle.left = area.X;
                rectangle.top = area.Y;
                rectangle.right = area.X + area.Width;
                rectangle.bottom = area.Y + area.Height;
                DDBLTFX effects;
                memset(&effects, 0, sizeof(effects));
                effects.dwSize = sizeof(effects);
                effects.dwFillColor = color;
                return VideoSurfacePtr->Blt(&rectangle, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &effects) == DD_OK;
            }
            return false;
        }
        return XSurface::Fill_Rect(cliprect, fillrect, color);
    }
    return false;
}


// bool DSurface::Fill_Rect_Trans(Rect const &rect, RGBClass const &color, unsigned int percent)
// (0x004AB730, slot 7)
// Blends color over the part of rect on the surface, percent (at most 100)
// of the way: each channel becomes (background * (255 - a) + color * a) / 256
// with a = percent * 255 / 100, kept to its bits of the screen pixel format.
// 16-bit surfaces only.

bool DSurface::Fill_Rect_Trans(Rect const &rect, RGBClass const &color, unsigned int percent)
{
    if (Bytes_Per_Pixel() < 2) return false;
    if (!rect.Is_Valid()) return false;
    Rect area = Get_Rect().Intersect(rect);
    if (!area.Is_Valid()) return false;
    unsigned short red_mask = 0xFF;
    red_mask >>= unknown_shift_852fbc;
    red_mask <<= unknown_shift_852fb8;
    unsigned short green_mask = 0xFF;
    green_mask >>= unknown_shift_852fcc;
    green_mask <<= unknown_shift_852fc8;
    unsigned short blue_mask = 0xFF;
    blue_mask >>= unknown_shift_852fc4;
    blue_mask <<= unknown_shift_852fc0;
    unsigned short *pixels = (unsigned short *)Lock(area.Top_Left());
    if (!pixels) return false;
    if (percent > 100) percent = 100;
    percent = percent * 255 / 100;
    unsigned short inverse = (unsigned short)(255 - percent);
    unsigned short pixel = (unsigned short)(((unsigned short)(unsigned char)(color.Red >> unknown_shift_852fbc) << unknown_shift_852fb8)
         | ((unsigned short)(unsigned char)(color.Green >> unknown_shift_852fcc) << unknown_shift_852fc8)
         | ((unsigned short)(unsigned char)(color.Blue >> unknown_shift_852fc4) << unknown_shift_852fc0));
    for (int y = 0; y < area.Height; ++y) {
        int offset = (Stride() / 2) * y;
        for (int x = 0; x < area.Width; ++x) {
            unsigned short background = pixels[offset];
            pixels[offset++] = (unsigned short)(((((background & red_mask) * inverse + (pixel & red_mask) * percent) >> 8) & red_mask)
                | ((((background & green_mask) * inverse + (pixel & green_mask) * percent) >> 8) & green_mask)
                | (((background & blue_mask) * inverse + (pixel & blue_mask) * percent) >> 8));
        }
    }
    Unlock();
    return true;
}


// void Build_Brightness_Tables(unsigned short *table, int count, PaletteClass const &palette) (0x004ABA00)
// Builds count 256-entry screen pixel tables of palette, table n scaled by
// 2n / (count - 1) in 16.16 fixed point (a single table is unscaled), each
// channel saturating at 255. The name is a hypothesis.

void __fastcall Build_Brightness_Tables(unsigned short *table, int count, PaletteClass const &palette)
{
    if (count < 1) {
        return;
    }
    for (int shade = 0; shade <= count - 1; shade++) {
        int scale;
        if (count > 1) {
            scale = (shade * 0x20000) / (count - 1);
        } else {
            scale = 0x10000;
        }
        for (int index = 0; index < 256; index++) {
            RGBClass const &color = palette[index];
            int red = (color.Red * scale) >> 16;
            int green = (color.Green * scale) >> 16;
            int blue = (color.Blue * scale) >> 16;
            if (red >= 255) {
                red = 255;
            }
            if (green >= 255) {
                green = 255;
            }
            if (blue >= 255) {
                blue = 255;
            }
            *table++ = (unsigned short)Build_Hicolor_Pixel(red, green, blue);
        }
    }
}


// int Get_Red_Left() (0x004ABB30)
// Returns unknown_shift_852fb8 (0x00852FB8); see shared/hicolor.hpp.

int Get_Red_Left()
{
    return unknown_shift_852fb8;
}


// int Get_Red_Right() (0x004ABB40)
// Returns unknown_shift_852fbc (0x00852FBC); see shared/hicolor.hpp.

int Get_Red_Right()
{
    return unknown_shift_852fbc;
}


// int Get_Green_Left() (0x004ABB50)
// Returns unknown_shift_852fc8 (0x00852FC8); see shared/hicolor.hpp.

int Get_Green_Left()
{
    return unknown_shift_852fc8;
}


// int Get_Green_Right() (0x004ABB60)
// Returns unknown_shift_852fcc (0x00852FCC); see shared/hicolor.hpp.

int Get_Green_Right()
{
    return unknown_shift_852fcc;
}


// int Get_Blue_Left() (0x004ABB70)
// Returns unknown_shift_852fc0 (0x00852FC0); see shared/hicolor.hpp.

int Get_Blue_Left()
{
    return unknown_shift_852fc0;
}


// int Get_Blue_Right() (0x004ABB80)
// Returns unknown_shift_852fc4 (0x00852FC4); see shared/hicolor.hpp.

int Get_Blue_Right()
{
    return unknown_shift_852fc4;
}


// int __fastcall Get_Unknown_7d6290() (0x004ABB90)
// Returns unknown_7d6290 (0x007D6290); see shared/hicolor.hpp.

int __fastcall Get_Unknown_7d6290()
{
    return unknown_7d6290;
}


// Draw a clipped 16-bit line between two screen points with depths (TibSun
// dsurface.cpp Draw_Depth_Glow_Line): each pixel in front of the depth ring
// (DepthBuffer 0x00839C6C) has every channel brightened by glow_strength/256,
// clamped to 255; write_depth stores the line's depth. The depths of clipped
// endpoints are interpolated by length. The px, origend, py order matches the
// original's register choice at the steep loop's depth test.
// Point length through the table square root, expanded inline (the TibSun
// point.h Length; Point2D::Length 0x0047D060 is the out-of-line copy).
static inline int Inline_Length(Point2D const &point)
{
    double y = (double)point.Y;
    double x = (double)point.X;
    return (int)Fast_Sqrt(x * x + y * y);
}

bool DSurface::Draw_Depth_Glow_Line(Rect const & cliprect, Point2D const & startpoint, Point2D const & endpoint, int glow_strength, int start_depth, int end_depth, bool write_depth)
{
    Point2D start;
    Point2D end;
    Rect rect = cliprect.Intersect(Get_Rect());
    if (rect.Width != 0 || rect.Height != 0) {
    start = Offset_Point(startpoint, rect);
    end = Offset_Point(endpoint, rect);
    int z1 = start_depth;
    int z2 = end_depth;
    if (start.X > end.X) {
        Point2D temp = start;
        start = end;
        end = temp;
        z1 = end_depth;
        z2 = start_depth;
    }
    int px = start.X;
    Point2D origend = end;
    int py = start.Y;
    if (Clip_Line(start, end, rect)) {
    int zend = z2;
    int zstart = z1;
    if (end.X != origend.X || end.Y != origend.Y) {
        Point2D clipped = Point2D(px, py) - end;
        Point2D full = Point2D(px, py) - origend;
        int fulllen = full.Length();
        unsigned int amount = abs((int)((double)clipped.Length() / (double)fulllen * (double)(z1 - z2)));
        if (z1 < z2) {
            zend = z1 + amount;
        } else {
            zend = z1 - amount;
        }
    }
    if (start.X != px || start.Y != py) {
        Point2D clipped_point(start.X - origend.X, start.Y - origend.Y);
        int clipped = Inline_Length(clipped_point);
        Point2D full_point(px - origend.X, py - origend.Y);
        int full = Inline_Length(full_point);
        unsigned int amount = abs((int)((double)clipped / (double)full * (double)(z1 - z2)));
        if (z2 > z1) {
            zstart = z2 - amount;
        } else {
            zstart = z2 + amount;
        }
    }
    Bytes_Per_Pixel();
    void * buffer = Lock(start);
    if (DepthBuffer != 0) {
    Point2D point = start;
    point.Y -= DepthBuffer->Area.Y;
    unsigned short * zbuffer = DepthBuffer->Get_Buffer_Offset(point);
    int start_y = start.Y;
    int zwrap = -1;
    ZBuffer *depth_buffer = DepthBuffer;
    unsigned short z = (unsigned short)(zstart + (short)(depth_buffer->Area.Y + depth_buffer->Get_Scroll()) - start_y - rect.Y);
    int zwidth = depth_buffer->Width;
    if (buffer != 0) {
    int dx = end.X - start.X;
    int xcount = dx;
    int dy = end.Y - start.Y;
    int dz = zend - zstart;
    int pitch = Stride();
    if (dy < 0) {
        pitch = -pitch;
        zwidth = -zwidth;
        zwrap = 1;
    }
    int zstep = 1;
    int abs_dy = abs(dy);
    if (dz < 0) {
        zstep = -1;
    }
    int dy2 = 2 * abs_dy;
    int abs_dz = abs(dz);
    int dx2 = 2 * dx;
    int dz2 = 2 * abs_dz;
    if (abs_dz > dx && abs_dz > abs_dy) {
        int xdelta = dy2 - abs_dz;
        int ydelta = dx2 - abs_dz;
        if (abs_dz > 0) {
            int offset = 0;
            for (int i = abs_dz; i != 0; i--) {
                if ((unsigned short)z < *zbuffer) {
                    RGBClass rgb = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + offset));
                    int red = rgb.Red;
                    red += (glow_strength * red) >> 8;
                    int green = rgb.Green;
                    green += (glow_strength * green) >> 8;
                    int blue = rgb.Blue;
                    blue += (glow_strength * blue) >> 8;
                    *(unsigned short *)((char *)buffer + offset) = (unsigned short)Build_Hicolor_Pixel(min(255, red), min(255, green), min(255, blue));
                    if (write_depth) {
                        *zbuffer = (unsigned short)z;
                    }
                }
                if (xdelta > 0) {
                    buffer = (unsigned char *)buffer + pitch;
                    xdelta -= dz2;
                    zbuffer += zwidth;
                    if (zwidth > 0) {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    } else {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
                    }
                    z += zwrap;
                }
                if (ydelta > 0) {
                    zbuffer++;
                    offset += 2;
                    zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    ydelta -= dz2;
                }
                z += zstep;
                ydelta += dx2;
                xdelta += dy2;
            }
        }
    } else if (dx > abs_dy) {
        int offset = 0;
        int ydelta = dy2 - dx;
        int zdelta = dz2 - dx;
        if (dx > 0) {
            do {
                if ((unsigned short)z < *zbuffer) {
                    RGBClass rgb = Deconstruct_Hicolor_Pixel(*((unsigned short *)buffer + offset));
                    int red = rgb.Red;
                    red += (glow_strength * red) >> 8;
                    int green = rgb.Green;
                    green += (glow_strength * green) >> 8;
                    int blue = rgb.Blue;
                    blue += (glow_strength * blue) >> 8;
                    *((unsigned short *)buffer + offset) = (unsigned short)Build_Hicolor_Pixel(min(255, red), min(255, green), min(255, blue));
                    if (write_depth) {
                        *zbuffer = (unsigned short)z;
                    }
                }
                if (ydelta > 0) {
                    buffer = (unsigned char *)buffer + pitch;
                    ydelta -= dx2;
                    zbuffer += zwidth;
                    if (zwidth > 0) {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    } else {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
                    }
                    z += zwrap;
                }
                if (zdelta > 0) {
                    z += zstep;
                    zdelta -= dx2;
                }
                zbuffer++;
                ydelta += dy2;
                zdelta += dz2;
                zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
            } while (++offset < xcount);
        }
    } else {
        int xdelta = dx2 - abs_dy;
        int zdelta = dz2 - abs_dy;
        if (abs_dy > 0) {
            int offset = 0;
            for (int i = abs_dy; i != 0; i--) {
                if ((unsigned short)z < *zbuffer) {
                    RGBClass rgb = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + offset));
                    int red = rgb.Red;
                    red += (glow_strength * red) >> 8;
                    int green = rgb.Green;
                    green += (glow_strength * green) >> 8;
                    int blue = rgb.Blue;
                    blue += (glow_strength * blue) >> 8;
                    *(unsigned short *)((char *)buffer + offset) = (unsigned short)Build_Hicolor_Pixel(min(255, red), min(255, green), min(255, blue));
                    if (write_depth) {
                        *zbuffer = (unsigned short)z;
                    }
                }
                if (xdelta > 0) {
                    zbuffer++;
                    offset += 2;
                    zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    xdelta -= dy2;
                }
                if (zdelta > 0) {
                    z += zstep;
                    zdelta -= dy2;
                }
                xdelta += dx2;
                zdelta += dz2;
                buffer = (unsigned char *)buffer + pitch;
                zbuffer += zwidth;
                if (zwidth > 0) {
                    zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                } else {
                    zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
                }
                z += zwrap;
            }
        }
    }
    Unlock();
    return(true);
    }
    }
    }
    }
    return(false);
}


// Not exact (RA2_NON_MATCHING): DSurface::Draw_Depth_Antialiased_Line at 0x004AC650, best score 35.58, 2026-11-13; differs: 1619 instructions in the original, 1585 in the draft (shape 82.96)
#ifdef RA2_NON_MATCHING
// Draw an antialiased clipped 16-bit line between two screen points with
// depths (TibSun dsurface.cpp Draw_Depth_Antialiased_Line): each step blends
// color into the pixel and its neighbour by the line's coverage, through a
// cubic gradient table, where the line is in front of the depth ring
// (DepthBuffer 0x00839C6C) and the alpha ring (AlphaBuffer 0x008315B4) is
// non-zero; the blend_* flags pick the channels, intensity scales the weights.
#include "shared/crt_math.hpp"
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

bool DSurface::Draw_Depth_Antialiased_Line(Rect const & cliprect, Point2D const & startpoint, Point2D const & endpoint, RGBClass const & color, int start_depth, int end_depth, bool write_depth, bool blend_red, bool blend_green, bool blend_blue, float intensity)
{
    static int GradientTable[257];
    static bool onetime = false;
    if (!onetime) {
        onetime = true;
        for (int i = 0; i <= 256; i++) {
            GradientTable[i] = (int)((pow((double)i / 128.0 - 1.0, 3.0) + 1.0) * 128.0);
        }
    }
    Rect surfrect = Get_Rect();
    surfrect.X += 1;
    surfrect.Y += 1;
    surfrect.Width -= 2;
    surfrect.Height -= 2;
    Rect clip = cliprect.Intersect(surfrect);
    if (clip.Width == 0 && clip.Height == 0) {
        return(false);
    }
    int red = color.Get_Red();
    int green = color.Get_Green();
    int blue = color.Get_Blue();
    Point2D start = Offset_Point(startpoint, clip);
    Point2D end = Offset_Point(endpoint, clip);
    int astart = start_depth;
    int aend = end_depth;
    if (start.X > end.X) {
        Point2D temp = start;
        start = end;
        end = temp;
        astart = end_depth;
        aend = start_depth;
    }
    Point2D origend = end;
    int px = start.X;
    int py = start.Y;
    if (Clip_Line(start, end, clip)) {
    int alpha_end = aend;
    int alpha_start = astart;
    if (end.X != origend.X || end.Y != origend.Y) {
        Point2D clipped = Point2D(px, py) - end;
        Point2D full = Point2D(px, py) - origend;
        int fulllen = full.Length();
        unsigned int amount = abs((int)((double)clipped.Length() / (double)fulllen * (double)(astart - aend)));
        if (astart < aend) {
            alpha_end = astart + amount;
        } else {
            alpha_end = astart - amount;
        }
    }
    if (start.X != px || start.Y != py) {
        Point2D clipped_point = start - origend;
        Point2D full_point = Point2D(px, py) - origend;
        int fulllen = full_point.Length();
        int clipped = clipped_point.Length();
        unsigned int amount = abs((int)((double)clipped / (double)fulllen * (double)(astart - aend)));
        if (aend > astart) {
            alpha_start = aend - amount;
        } else {
            alpha_start = aend + amount;
        }
    }
    void * buffer = Lock(start);
    if (DepthBuffer == 0) {
        Unlock();
        return(false);
    }
    Point2D point = start;
    point.Y -= DepthBuffer->Area.Y;
    unsigned short * zbuffer = DepthBuffer->Get_Buffer_Offset(point);
    unsigned short * abuffer = AlphaBuffer->Get_Buffer_Offset(Point2D(start.X - AlphaBuffer->Area.X, start.Y - AlphaBuffer->Area.Y));
    int start_y = start.Y;
    int zwrap = -1;
    ZBuffer *depth_buffer = DepthBuffer;
    unsigned short z = (unsigned short)(alpha_start + (short)(depth_buffer->Area.Y + depth_buffer->Get_Scroll()) - start_y - clip.Y);
    int zwidth = depth_buffer->Width;
    if (buffer != 0) {
    int dy = end.Y - start.Y;
    int dx = end.X - start.X;
    int xcount = end.X - start.X;
    int adelta = alpha_end - alpha_start;
    int pitch = Stride();
    if (dy < 0) {
        pitch = -pitch;
        zwidth = -zwidth;
        zwrap = 1;
    }
    int astep = 1;
    int abs_dy = abs(dy);
    int half_pitch = pitch / 2;
    if (adelta < 0) {
        astep = -1;
    }
    int dy2 = 2 * abs_dy;
    int abs_adelta = abs(adelta);
    int adelta2 = 2 * abs_adelta;
    if (dx == 0 && abs_dy == 0) {
        Unlock();
        return(false);
    }
    if (abs_adelta > dx && abs_adelta > abs_dy) {
        int weight = 256;
        if (dx > abs_dy) {
            int grad = (int)((double)abs_dy / (double)xcount * 256.0);
            int xoff = 0;
            if (dx > 0) {
                int otherweight = 0;
                int neg_grad = -grad;
                int neighoff = 2 * half_pitch;
                do {
                    int cov = *abuffer;
                    if (z < *zbuffer) {
                        if (cov) {
                            int lidx = (int)((double)weight * intensity);
                            int oidx = (int)((double)otherweight * intensity);
                            int lf = GradientTable[lidx];
                            int lbk = GradientTable[256 - lidx];
                            int nf = GradientTable[oidx];
                            int nbk = GradientTable[256 - oidx];
                            RGBClass line = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + 2 * xoff));
                            RGBClass neighbor = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + neighoff));
                            int outlg = line.Get_Green();
                            int outng = neighbor.Get_Green();
                            int outlr = line.Get_Red();
                            int outnr = neighbor.Get_Red();
                            int outlb = line.Get_Blue();
                            int outnb = neighbor.Get_Blue();
                            if (blend_red) {
                                outlr = (cov * (red * nf + outlr * nbk)) >> 15;
                                outnr = (cov * (red * lf + outnr * lbk)) >> 15;
                            }
                            if (blend_green) {
                                outlg = (cov * (green * nf + outlg * nbk)) >> 15;
                                outng = (cov * (green * lf + outng * lbk)) >> 15;
                            }
                            if (blend_blue) {
                                outlb = (cov * (blue * nf + outlb * nbk)) >> 15;
                                outnb = (cov * (blue * lf + outnb * lbk)) >> 15;
                            }
                            int wb = MIN(255, outnb);
                            if (outng > 255) {
                                outng = 255;
                            }
                            if (outnr > 255) {
                                outnr = 255;
                            }
                            *(unsigned short *)((char *)buffer + 2 * xoff) = (unsigned short)Build_Hicolor_Pixel(outnr, outng, wb);
                            if (outlb > 255) {
                                outlb = 255;
                            }
                            if (outlg > 255) {
                                outlg = 255;
                            }
                            if (outlr > 255) {
                                outlr = 255;
                            }
                            *(unsigned short *)((char *)buffer + neighoff) = (unsigned short)Build_Hicolor_Pixel(outlr, outlg, outlb);
                            if (write_depth) {
                                *zbuffer = z;
                            }
                        }
                    }
                    weight -= grad;
                    otherweight -= neg_grad;
                    if (otherweight > 256) {
                        otherweight -= 256;
                        buffer = (unsigned char *)buffer + pitch;
                        weight += 256;
                        zbuffer += zwidth;
                        if (zwidth > 0) {
                            zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                        } else {
                            zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
                        }
                        abuffer += zwidth;
                        if (zwidth > 0) {
                            abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
                        } else {
                            abuffer = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)abuffer);
                        }
                        z += zwrap;
                    }
                    xoff++;
                    neighoff += 2;
                    zbuffer++;
                    zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    abuffer++;
                    abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
                    z += astep;
                    xoff++;
                    neighoff += 2;
                } while (xoff < xcount);
            }
        } else {
            int grad = (int)((double)xcount / (double)abs_dy * 256.0);
            if (abs_dy > 0) {
                int otherweight = 0;
                int neg_grad = -grad;
                int i = abs_dy;
                int byteoff = 0;
                int yadjust = zwrap + astep;
                do {
                    int cov = *abuffer;
                    if (z < *zbuffer) {
                        if (cov) {
                            int lidx = (int)((double)weight * intensity);
                            int oidx = (int)((double)otherweight * intensity);
                            int lf = GradientTable[lidx];
                            int lbk = GradientTable[256 - lidx];
                            int nf = GradientTable[oidx];
                            int nbk = GradientTable[256 - oidx];
                            RGBClass line = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + byteoff));
                            RGBClass neighbor = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + byteoff + 2));
                            int outlb = line.Get_Blue();
                            int outnb = neighbor.Get_Blue();
                            int outnr = neighbor.Get_Red();
                            int outlr = line.Get_Red();
                            int outng = neighbor.Get_Green();
                            int outlg = line.Get_Green();
                            if (blend_red) {
                                outnr = (cov * (red * nf + outnr * nbk)) >> 15;
                                outlr = (cov * (red * lf + outlr * lbk)) >> 15;
                            }
                            if (blend_green) {
                                outng = (cov * (green * nf + outng * nbk)) >> 15;
                                outlg = (cov * (green * lf + outlg * lbk)) >> 15;
                            }
                            if (blend_blue) {
                                outnb = (cov * (blue * nf + outnb * nbk)) >> 15;
                                outlb = (cov * (blue * lf + outlb * lbk)) >> 15;
                            }
                            int wb = 255;
                            if (outlb <= 255) {
                                wb = outlb;
                            }
                            if (outlg > 255) {
                                outlg = 255;
                            }
                            if (outlr > 255) {
                                outlr = 255;
                            }
                            *(unsigned short *)((char *)buffer + byteoff) = (unsigned short)Build_Hicolor_Pixel(outlr, outlg, wb);
                            if (outnb > 255) {
                                outnb = 255;
                            }
                            if (outng > 255) {
                                outng = 255;
                            }
                            if (outnr > 255) {
                                outnr = 255;
                            }
                            *(unsigned short *)((char *)buffer + byteoff + 2) = (unsigned short)Build_Hicolor_Pixel(outnr, outng, outnb);
                            if (write_depth) {
                                *zbuffer = z;
                            }
                        }
                    }
                    weight -= grad;
                    otherweight -= neg_grad;
                    if (otherweight > 256) {
                        otherweight -= 256;
                        weight += 256;
                        byteoff += 2;
                        zbuffer++;
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                        abuffer++;
                        abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
                    }
                    buffer = (unsigned char *)buffer + pitch;
                    zbuffer += zwidth;
                    if (zwidth > 0) {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    } else {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
                    }
                    abuffer += zwidth;
                    if (zwidth > 0) {
                        abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
                    } else {
                        abuffer = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)abuffer);
                    }
                    z += yadjust;
                    i--;
                } while (i != 0);
            }
        }
    } else {
        int weight = 256;
        if (dx > abs_dy) {
            int adda = adelta2 - dx;
            int grad = (int)((double)abs_dy / (double)xcount * 256.0);
            int xoff = 0;
            if (dx > 0) {
                int neg_grad = -grad;
                int neighoff = 2 * half_pitch;
                int otherweight = 0;
                do {
                    int cov = *abuffer;
                    if (z < *zbuffer) {
                        if (cov) {
                            int lidx = (int)((double)weight * intensity);
                            int oidx = (int)((double)otherweight * intensity);
                            int lf = GradientTable[lidx];
                            int lbk = GradientTable[256 - lidx];
                            int nf = GradientTable[oidx];
                            int nbk = GradientTable[256 - oidx];
                            RGBClass line = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + 2 * xoff));
                            RGBClass neighbor = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + neighoff));
                            int outnr = neighbor.Get_Red();
                            int outlr = line.Get_Red();
                            int outng = neighbor.Get_Green();
                            int outlg = line.Get_Green();
                            int outnb = neighbor.Get_Blue();
                            int outlb = line.Get_Blue();
                            if (blend_red) {
                                outlr = (cov * (red * nf + outlr * nbk)) >> 15;
                                outnr = (cov * (red * lf + outnr * lbk)) >> 15;
                            }
                            if (blend_green) {
                                outlg = (cov * (green * nf + outlg * nbk)) >> 15;
                                outng = (cov * (green * lf + outng * lbk)) >> 15;
                            }
                            if (blend_blue) {
                                outlb = (cov * (blue * nf + outlb * nbk)) >> 15;
                                outnb = (cov * (blue * lf + outnb * lbk)) >> 15;
                            }
                            int wb = 255;
                            if (outnb <= 255) {
                                wb = outnb;
                            }
                            if (outng > 255) {
                                outng = 255;
                            }
                            if (outnr > 255) {
                                outnr = 255;
                            }
                            *(unsigned short *)((char *)buffer + 2 * xoff) = (unsigned short)Build_Hicolor_Pixel(outnr, outng, wb);
                            if (outlb > 255) {
                                outlb = 255;
                            }
                            if (outlg > 255) {
                                outlg = 255;
                            }
                            if (outlr > 255) {
                                outlr = 255;
                            }
                            *(unsigned short *)((char *)buffer + neighoff) = (unsigned short)Build_Hicolor_Pixel(outlr, outlg, outlb);
                            if (write_depth) {
                                *zbuffer = z;
                            }
                        }
                    }
                    weight -= grad;
                    otherweight -= neg_grad;
                    if (otherweight > 256) {
                        otherweight -= 256;
                        weight += 256;
                        buffer = (unsigned char *)buffer + pitch;
                        zbuffer += zwidth;
                        if (zwidth > 0) {
                            zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                        } else {
                            zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
                        }
                        abuffer += zwidth;
                        if (zwidth > 0) {
                            abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
                        } else {
                            abuffer = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)abuffer);
                        }
                        z += zwrap;
                    }
                    if (adda > 0) {
                        z += astep;
                        adda -= 2 * xcount;
                    }
                    adda += adelta2;
                    zbuffer++;
                    zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    abuffer++;
                    abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
                    xoff++;
                    neighoff += 2;
                } while (xoff < xcount);
            }
        } else {
            int adda = adelta2 - abs_dy;
            int grad = (int)((double)xcount / (double)abs_dy * 256.0);
            if (abs_dy > 0) {
                int byteoff = 0;
                int neg_grad = -grad;
                int otherweight = 0;
                int i = abs_dy;
                do {
                    int cov = *abuffer;
                    if (z < *zbuffer) {
                        if (cov) {
                            int lidx = (int)((double)weight * intensity);
                            int oidx = (int)((double)otherweight * intensity);
                            int lf = GradientTable[lidx];
                            int lbk = GradientTable[256 - lidx];
                            int nf = GradientTable[oidx];
                            int nbk = GradientTable[256 - oidx];
                            RGBClass line = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + byteoff));
                            RGBClass neighbor = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + byteoff + 2));
                            int outlb = line.Get_Blue();
                            int outnb = neighbor.Get_Blue();
                            int outlr = line.Get_Red();
                            int outnr = neighbor.Get_Red();
                            int outng = neighbor.Get_Green();
                            int outlg = line.Get_Green();
                            if (blend_red) {
                                outlr = (cov * (red * lf + outlr * lbk)) >> 15;
                                outnr = (cov * (red * nf + outnr * nbk)) >> 15;
                            }
                            if (blend_green) {
                                outlg = (cov * (green * lf + outlg * lbk)) >> 15;
                                outng = (cov * (green * nf + outng * nbk)) >> 15;
                            }
                            if (blend_blue) {
                                outlb = (cov * (blue * lf + outlb * lbk)) >> 15;
                                outnb = (cov * (blue * nf + outnb * nbk)) >> 15;
                            }
                            int wb = MIN(255, outlb);
                            if (outlg > 255) {
                                outlg = 255;
                            }
                            if (outlr > 255) {
                                outlr = 255;
                            }
                            *(unsigned short *)((char *)buffer + byteoff) = (unsigned short)Build_Hicolor_Pixel(outlr, outlg, wb);
                            if (outnb > 255) {
                                outnb = 255;
                            }
                            if (outng > 255) {
                                outng = 255;
                            }
                            if (outnr > 255) {
                                outnr = 255;
                            }
                            *(unsigned short *)((char *)buffer + byteoff + 2) = (unsigned short)Build_Hicolor_Pixel(outnr, outng, outnb);
                            if (write_depth) {
                                *zbuffer = z;
                            }
                        }
                    }
                    weight -= grad;
                    otherweight -= neg_grad;
                    if (otherweight > 256) {
                        otherweight -= 256;
                        weight += 256;
                        byteoff += 2;
                        zbuffer++;
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                        abuffer++;
                        abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
                    }
                    if (adda > 0) {
                        z += astep;
                        adda -= dy2;
                    }
                    adda += adelta2;
                    buffer = (unsigned char *)buffer + pitch;
                    z += zwrap;
                    zbuffer += zwidth;
                    if (zwidth > 0) {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    } else {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
                    }
                    abuffer += zwidth;
                    if (zwidth > 0) {
                        abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
                    } else {
                        abuffer = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)abuffer);
                    }
                    i--;
                } while (i != 0);
            }
        }
    }
    Unlock();
    return(true);
    }
    Unlock();
    return(false);
    }
    return(false);
}
#endif

// DSurface slot 16 (0x004ADE00; an RA2 addition, not in TibSun). The TibSun
// Draw_Depth_Glow_Line / Draw_Depth_Shaded_Line skeleton: clip, interpolate the
// clipped depths by length, then step along the major axis. Where the line is in
// front of the depth ring (0x00839C6C) the background pixel is brightened through
// Brighten_Hicolor_Pixel by color * intensity scaled by the level ring (0x008315B4,
// level/128). Nothing is drawn when every scaled channel is below 8. The level
// pointer follows row steps only (x steps move the depth pointer alone).
// Point length through the table square root, expanded inline (the squared Y
// term is formed first).
static inline int Depth_Line_Length(Point2D const &point)
{
    double x = (double)point.X;
    double y = (double)point.Y;
    double yy = y * y;
    return (int)Fast_Sqrt(yy + x * x);
}

bool DSurface::Unknown_Slot16(Rect const & cliprect, Point2D const & startpoint, Point2D const & endpoint, RGBClass const & color, float intensity, int start_depth, int end_depth)
{
    int red = (int)(color.Red * intensity);
    int green = (int)(color.Green * intensity);
    int blue = (int)(color.Blue * intensity);
    if (red >= 8 || green >= 8 || blue >= 8) {
    Point2D start;
    Point2D end;
    Rect rect = cliprect.Intersect(Get_Rect());
    if (rect.Width != 0 || rect.Height != 0) {
    start = Offset_Point(startpoint, rect);
    end = Offset_Point(endpoint, rect);
    int z1 = start_depth;
    int z2 = end_depth;
    if (start.X > end.X) {
        Point2D temp = start;
        start = end;
        end = temp;
        z1 = end_depth;
        z2 = start_depth;
    }
    int px = start.X;
    int py = start.Y;
    Point2D origend = end;
    if (Clip_Line(start, end, rect)) {
    int zend = z2;
    int zstart = z1;
    if (end.X != origend.X || end.Y != origend.Y) {
        Point2D clipped_point(px - end.X, py - end.Y);
        int clipped = Depth_Line_Length(clipped_point);
        Point2D full_point(px - origend.X, py - origend.Y);
        int full = Depth_Line_Length(full_point);
        unsigned int amount = abs((int)((double)clipped / (double)full * (double)(z1 - z2)));
        if (z1 < z2) {
            zend = z1 + amount;
        } else {
            zend = z1 - amount;
        }
    }
    if (start.X != px || start.Y != py) {
        Point2D clipped_point(start.X - origend.X, start.Y - origend.Y);
        int clipped = Depth_Line_Length(clipped_point);
        Point2D full_point(px - origend.X, py - origend.Y);
        int full = Depth_Line_Length(full_point);
        unsigned int amount = abs((int)((double)clipped / (double)full * (double)(z1 - z2)));
        if (z2 > z1) {
            zstart = z2 - amount;
        } else {
            zstart = z2 + amount;
        }
    }
    Bytes_Per_Pixel();
    void * buffer = Lock(start);
    if (DepthBuffer != 0) {
    Point2D point = start;
    point.Y -= DepthBuffer->Area.Y;
    unsigned short * zbuffer = DepthBuffer->Get_Buffer_Offset(point);
    point = start;
    point.Y -= AlphaBuffer->Area.Y;
    unsigned short * level = AlphaBuffer->Get_Buffer_Offset(point);
    int start_y = start.Y;
    int zwrap = -1;
    ZBuffer *depth_buffer = DepthBuffer;
    unsigned short z = (unsigned short)(zstart + (short)(depth_buffer->Area.Y + depth_buffer->Get_Scroll()) - start_y - rect.Y);
    int zwidth = depth_buffer->Width;
    if (buffer != 0) {
    int dx = end.X - start.X;
    int dy = end.Y - start.Y;
    int dz = zend - zstart;
    int pitch = Stride();
    if (dy < 0) {
        pitch = -pitch;
        zwidth = -zwidth;
        zwrap = 1;
    }
    int zstep = 1;
    int abs_dy = abs(dy);
    if (dz < 0) {
        zstep = -1;
    }
    RGBClass back;
    int abs_dz = abs(dz);
    int dx2 = 2 * dx;
    int dy2 = 2 * abs_dy;
    int dz2 = 2 * abs_dz;
    if (abs_dz > dx && abs_dz > abs_dy) {
        int xdelta = dy2 - abs_dz;
        int ydelta = dx2 - abs_dz;
        if (abs_dz > 0) {
            int offset = 0;
            for (int i = abs_dz; i != 0; i--) {
                int v = *level;
                if (z < *zbuffer) {
                    back = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + offset));
                    *(unsigned short *)((char *)buffer + offset) = (unsigned short)Brighten_Hicolor_Pixel(back, (v * red) >> 7, (v * green) >> 7, (v * blue) >> 7);
                }
                if (xdelta > 0) {
                    buffer = (unsigned char *)buffer + pitch;
                    xdelta -= dz2;
                    zbuffer += zwidth;
                    if (zwidth > 0) {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    } else {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
                    }
                    level += zwidth;
                    if (zwidth > 0) {
                        level = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)level);
                    } else {
                        level = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)level);
                    }
                    z += zwrap;
                }
                if (ydelta > 0) {
                    zbuffer++;
                    offset += 2;
                    zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    ydelta -= dz2;
                }
                z += zstep;
                ydelta += dx2;
                xdelta += dy2;
            }
        }
    } else if (dx > abs_dy) {
        int ydelta = dy2 - dx;
        int zdelta = dz2 - dx;
        int i = 0;
        if (dx > 0) {
            for (; i < dx; i++) {
                int v = *level;
                if (z < *zbuffer) {
                    back = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + 2 * i));
                    *(unsigned short *)((char *)buffer + 2 * i) = (unsigned short)Brighten_Hicolor_Pixel(back, (v * red) >> 7, (v * green) >> 7, (v * blue) >> 7);
                }
                if (ydelta > 0) {
                    buffer = (unsigned char *)buffer + pitch;
                    ydelta -= 2 * dx;
                    zbuffer += zwidth;
                    if (zwidth > 0) {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    } else {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
                    }
                    level += zwidth;
                    if (zwidth > 0) {
                        level = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)level);
                    } else {
                        level = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)level);
                    }
                    z += zwrap;
                }
                if (zdelta > 0) {
                    z += zstep;
                    zdelta -= 2 * dx;
                }
                zbuffer++;
                ydelta += dy2;
                zdelta += dz2;
                zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
            }
        }
    } else {
        int xdelta = dx2 - abs_dy;
        int zdelta = 2 * abs_dz - abs_dy;
        if (abs_dy > 0) {
            int offset = 0;
            for (int i = abs_dy; i != 0; i--) {
                int v = *level;
                if (z < *zbuffer) {
                    back = Deconstruct_Hicolor_Pixel(*(unsigned short *)((char *)buffer + offset));
                    *(unsigned short *)((char *)buffer + offset) = (unsigned short)Brighten_Hicolor_Pixel(back, (v * red) >> 7, (v * green) >> 7, (v * blue) >> 7);
                }
                if (xdelta > 0) {
                    zbuffer++;
                    offset += 2;
                    zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    xdelta -= dy2;
                }
                if (zdelta > 0) {
                    z += zstep;
                    zdelta -= dy2;
                }
                xdelta += dx2;
                zdelta += dz2;
                buffer = (unsigned char *)buffer + pitch;
                zbuffer += zwidth;
                if (zwidth > 0) {
                    zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                } else {
                    zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
                }
                level += zwidth;
                if (zwidth > 0) {
                    level = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)level);
                } else {
                    level = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)level);
                }
                z += zwrap;
            }
        }
    }
    Unlock();
    return(true);
    }
    }
    }
    }
    }
    return(false);
}


// int Brighten_Hicolor_Pixel(RGBClass const &color, int red, int green, int blue) (0x004AE8D0)
// Adds red/green/blue to color. When one or two channels reach 255 their
// combined excess (halved for a single channel) is added to the channels
// still below 255; every channel is then clamped to 255 and the colour packed
// into a screen pixel. The name is a hypothesis.

int __fastcall Brighten_Hicolor_Pixel(RGBClass const &color, int red_add, int green_add, int blue_add)
{
    int red = color.Red + red_add;
    int green = color.Green + green_add;
    int blue = color.Blue + blue_add;
    int excess = 0;
    int saturated = 0;
    if (red >= 255) {
        saturated++;
        excess += red - 255;
    }
    if (green >= 255) {
        saturated++;
        excess += green - 255;
    }
    if (blue >= 255) {
        saturated++;
        excess += blue - 255;
    }
    if (saturated != 0 && saturated < 3) {
        if (saturated == 1) {
            excess >>= 1;
        }
        if (red < 255) {
            red += excess;
        }
        if (green < 255) {
            green += excess;
        }
        if (blue < 255) {
            blue += excess;
        }
    }
    if (blue > 255) {
        blue = 255;
    }
    if (green > 255) {
        green = 255;
    }
    if (red > 255) {
        red = 255;
    }
    return Build_Hicolor_Pixel(red, green, blue);
}


// bool DSurface::Draw_Trail_Line_4AE9C0(...) (0x004AE9C0; LineTrail::Draw 0x005395C0): a line from startpoint to
// endpoint clipped to cliprect, blended in color at intensity over the pixels whose depth passes, the depth
// interpolated from start_depth to end_depth (z, x and y major Bresenham walks over the screen, depth and alpha
// buffers). The depth test reads the buffer behind DepthBuffer and the level the one behind AlphaBuffer; ra2-decomp's
// YR twin calls those DepthBuffer and AlphaBuffer the other way round (name lead, not settled here).
// From ra2-decomp 0x004BEAC0 (YR identical twin, src/dsurface.cpp DSurface::FUN_004BEAC0), on Scout795's draft
// (its blend helper kept; ra2-decomp's Alpha_Hicolor_Pixel form scores 80.75):
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// TibSun/ra2-decomp's buffer wrap helpers (zbuffer.h / abuffer.h): a pointer stepped past the end of the
// circular buffer comes back to its start.
static inline unsigned short *Blit_Wrap_Z_Buffer(unsigned short *buf)
{
    return((unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)buf));
}
static inline unsigned short *Blit_Wrap_A_Buffer(unsigned short *buf)
{
    return((unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)buf));
}
// Actual source/background blend, followed by the ring brightness scale.
static inline unsigned short Trail_Blend_Pixel(unsigned short pixel, int level, int inverse, int red, int green, int blue)
{
    RGBClass const &background = Deconstruct_Hicolor_Pixel(pixel);
    return (unsigned short)Build_Hicolor_Pixel(
        ((((background.Red * inverse) >> 8) + red) * level) >> 7,
        ((((background.Green * inverse) >> 8) + green) * level) >> 7,
        ((((background.Blue * inverse) >> 8) + blue) * level) >> 7);
}

bool DSurface::Draw_Trail_Line_4AE9C0(Rect const & cliprect, Point2D const & startpoint, Point2D const & endpoint, RGBClass const & color, int intensity, int start_depth, int end_depth)
{
    Point2D start;
    Point2D end;
    if (intensity >= 8) {
    Rect rect = cliprect.Intersect(Get_Rect());
    if (rect.Width != 0 || rect.Height != 0) {
    start = Offset_Point(startpoint, rect);
    end = Offset_Point(endpoint, rect);
    int z2 = end_depth;
    int z1 = start_depth;
    if (start.X > end.X) {
        Point2D temp = start;
        start = end;
        end = temp;
        z1 = end_depth;
        z2 = start_depth;
    }
    Point2D origend = end;
    int px = start.X;
    int py = start.Y;
    if (Clip_Line(start, end, rect)) {
    int zend = z2;
    int zstart = z1;
    if (end.X != origend.X || end.Y != origend.Y) {
        Point2D clipped_point = Point2D(px, py) - end;
        Point2D full_point = Point2D(px, py) - origend;
        int full = full_point.Length();
        int clipped = clipped_point.Length();
        unsigned int amount = abs((int)((double)clipped / (double)full * (double)(z1 - z2)));
        if (z1 < z2) {
            zend = z1 + amount;
        } else {
            zend = z1 - amount;
        }
    }
    if (start.X != px || start.Y != py) {
        Point2D clipped_point = start - origend;
        Point2D full_point = Point2D(px, py) - origend;
        int full = full_point.Length();
        int clipped = clipped_point.Length();
        unsigned int amount = abs((int)((double)clipped / (double)full * (double)(z1 - z2)));
        if (z2 > z1) {
            zstart = z2 - amount;
        } else {
            zstart = z2 + amount;
        }
    }
    Bytes_Per_Pixel();
    void * buffer = Lock(start);
    if (DepthBuffer != 0) {
    Point2D point = start;
    point.Y -= DepthBuffer->Area.Y;
    unsigned short * abuffer = DepthBuffer->Get_Buffer_Offset(point);
    point = start;
    point.Y -= AlphaBuffer->Area.Y;
    unsigned short * zbuffer = AlphaBuffer->Get_Buffer_Offset(point);
    int start_y = start.Y;
    int zwrap = -1;
    ZBuffer *depth_buffer = DepthBuffer;
    unsigned short z = (unsigned short)(zstart + (short)(depth_buffer->Area.Y + depth_buffer->Get_Scroll()) - start_y - rect.Y);
    int zwidth = DepthBuffer->Width;
    if (buffer != 0) {
    int red = (color.Red * intensity) >> 8;
    int green = (color.Green * intensity) >> 8;
    int blue = (color.Blue * intensity) >> 8;
    int inverse = 256 - intensity;
    int dx = end.X - start.X;
    int dy = end.Y - start.Y;
    int dz = zend - zstart;
    int pitch = Stride();
    if (dy < 0) {
        pitch = -pitch;
        zwidth = -zwidth;
        zwrap = 1;
    }
    int abs_dy = abs(dy);
    int zstep = 1;
    if (dz < 0) {
        zstep = -1;
    }
    int abs_dz = abs(dz);
    int dx2 = 2 * dx;
    int dy2 = 2 * abs_dy;
    int dz2 = 2 * abs_dz;
    if (abs_dz > dx && abs_dz > abs_dy) {
        int xdelta = dy2 - abs_dz;
        int ydelta = dx2 - abs_dz;
        if (abs_dz > 0) {
            int offset = 0;
            for (int i = abs_dz; i != 0; i--) {
                int v = *zbuffer;
                if (z < *abuffer && v != 0) {
                    *(unsigned short *)((char *)buffer + offset) = Trail_Blend_Pixel(*(unsigned short *)((char *)buffer + offset), v, inverse, red, green, blue);
                }
                if (xdelta > 0) {
                    buffer = (unsigned char *)buffer + pitch;
                    xdelta -= dz2;
                    abuffer += zwidth;
                    if (zwidth > 0) {
                        abuffer = Blit_Wrap_A_Buffer(abuffer);
                    } else {
                        abuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)abuffer);
                    }
                    zbuffer += zwidth;
                    if (zwidth > 0) {
                        zbuffer = Blit_Wrap_Z_Buffer(zbuffer);
                    } else {
                        zbuffer = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)zbuffer);
                    }
                    z += zwrap;
                }
                if (ydelta > 0) {
                    abuffer++;
                    offset += 2;
                    abuffer = Blit_Wrap_A_Buffer(abuffer);
                    zbuffer++;
                    zbuffer = Blit_Wrap_Z_Buffer(zbuffer);
                    ydelta -= dz2;
                }
                z += zstep;
                ydelta += dx2;
                xdelta += dy2;
            }
        }
    } else if (dx > abs_dy) {
        int ydelta = dy2 - dx;
        int zdelta = dz2 - dx;
        int i = 0;
        if (dx > 0) {
            for (; i < dx; i++) {
                int v = *zbuffer;
                if (z < *abuffer && v != 0) {
                    *(unsigned short *)((char *)buffer + 2 * i) = Trail_Blend_Pixel(*(unsigned short *)((char *)buffer + 2 * i), v, inverse, red, green, blue);
                }
                if (ydelta > 0) {
                    buffer = (unsigned char *)buffer + pitch;
                    ydelta -= 2 * dx;
                    abuffer += zwidth;
                    if (zwidth > 0) {
                        abuffer = Blit_Wrap_A_Buffer(abuffer);
                    } else {
                        abuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)abuffer);
                    }
                    z += zwrap;
                    zbuffer += zwidth;
                    if (zwidth > 0) {
                        zbuffer = Blit_Wrap_Z_Buffer(zbuffer);
                    } else {
                        zbuffer = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)zbuffer);
                    }
                }
                if (zdelta > 0) {
                    z += zstep;
                    zdelta -= 2 * dx;
                }
                abuffer++;
                ydelta += dy2;
                zdelta += dz2;
                abuffer = Blit_Wrap_A_Buffer(abuffer);
                zbuffer++;
                zbuffer = Blit_Wrap_Z_Buffer(zbuffer);
            }
        }
    } else {
        int xdelta = dx2 - abs_dy;
        int zdelta = dz2 - abs_dy;
        if (abs_dy > 0) {
            int offset = 0;
            for (int i = abs_dy; i != 0; i--) {
                int v = *zbuffer;
                if (z < *abuffer && v != 0) {
                    *(unsigned short *)((char *)buffer + offset) = Trail_Blend_Pixel(*(unsigned short *)((char *)buffer + offset), v, inverse, red, green, blue);
                }
                if (xdelta > 0) {
                    abuffer++;
                    offset += 2;
                    abuffer = Blit_Wrap_A_Buffer(abuffer);
                    zbuffer++;
                    zbuffer = Blit_Wrap_Z_Buffer(zbuffer);
                    xdelta -= dy2;
                }
                if (zdelta > 0) {
                    z += zstep;
                    zdelta -= dy2;
                }
                xdelta += dx2;
                zdelta += dz2;
                buffer = (unsigned char *)buffer + pitch;
                abuffer += zwidth;
                if (zwidth > 0) {
                    abuffer = Blit_Wrap_A_Buffer(abuffer);
                } else {
                    abuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)abuffer);
                }
                z += zwrap;
                zbuffer += zwidth;
                if (zwidth > 0) {
                    zbuffer = Blit_Wrap_Z_Buffer(zbuffer);
                } else {
                    zbuffer = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)zbuffer);
                }
            }
        }
    }
    Unlock();
    return(true);
    }
    }
    }
    }
    }
    return(false);
}


// unsigned short Blend_Hicolor_Pixel(unsigned short color, unsigned short background, int percent) (0x004AF550)
// Mixes two screen pixels for percent 0, 25, 50, 75 or 100 (any other value
// keeps color): 0 gives background, 50 the average of the two through the
// half-brightness mask. As compiled, 25 and 75 both give color itself
// masked to multiples of four per channel (the background is not read);
// the original expressions for those two cases are not known. The name is
// a hypothesis.

// The half mask is defined in this unit: the compiler then zero-extends it
// with a 32-bit load (the quarter mask, read as a word, is only declared).
unsigned short unknown_half_mask_852fd0;

unsigned short __fastcall Blend_Hicolor_Pixel(unsigned short color, unsigned short background, int percent)
{
    switch (percent) {
    case 75:
        return (unsigned short)(((color >> 2) & unknown_quarter_mask_852fd2) * 4);
    case 50:
        return (unsigned short)(((color >> 1) & unknown_half_mask_852fd0) + ((background >> 1) & unknown_half_mask_852fd0));
    case 25:
        return (unsigned short)(((color >> 2) & unknown_quarter_mask_852fd2) * 4);
    case 0:
        return background;
    case 100:
        return color;
    }
    return color;
}


// Draw a clipped 16-bit line whose colour moves between two palette colours:
// every pixel takes the colour at ratio between from and to, and ratio
// advances by step, bouncing between 0 and 1.

// The screen pixel of the colour at ratio between from and to.
inline unsigned short Gradient_Pixel(RGBClass const &from, RGBClass const &to, float const &ratio)
{
    RGBClass color = RGBClass(from, to, ratio);
    return (unsigned short)(((unsigned short)(unsigned char)(color.Red >> unknown_shift_852fbc) << unknown_shift_852fb8)
         | ((unsigned short)(unsigned char)(color.Green >> unknown_shift_852fcc) << unknown_shift_852fc8)
         | ((unsigned short)(unsigned char)(color.Blue >> unknown_shift_852fc4) << unknown_shift_852fc0));
}

// Advance ratio by step, reversing at 0 and 1.
inline void Step_Ratio(float &ratio, float &step)
{
    ratio += step;
    if (ratio < 0.0f && step < 0.0f) {
        ratio = 0.0f;
        step = -step;
    } else if (ratio > 1.0f && step > 0.0f) {
        ratio = 1.0f;
        step = -step;
    }
}

bool DSurface::Draw_Gradient_Line(Rect const &cliprect, Point2D const &startpoint, Point2D const &endpoint,
                                  RGBClass const &from, RGBClass const &to, float &step, float &ratio)
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
        if (Bytes_Per_Pixel() == 2) {
            unsigned short *pixels = (unsigned short *)Lock(start);
            if (pixels) {
                if (start.Y == end.Y) {
                    for (int index = 0; index <= end.X - start.X; ++index) {
                        pixels[index] = Gradient_Pixel(from, to, ratio);
                        Step_Ratio(ratio, step);
                    }
                } else if (start.X == end.X) {
                    int stride;
                    if (start.Y > end.Y) stride = -Stride();
                    else stride = Stride();
                    int height = abs(end.Y - start.Y);
                    for (int index = 0; index <= height; ++index) {
                        *pixels = Gradient_Pixel(from, to, ratio);
                        Step_Ratio(ratio, step);
                        pixels = (unsigned short *)((char *)pixels + stride);
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
                        for (int index = 0; index <= dx; ++index) {
                            pixels[index] = Gradient_Pixel(from, to, ratio);
                            Step_Ratio(ratio, step);
                            if (error > 0) {
                                pixels = (unsigned short *)((char *)pixels + stride);
                                error -= dx2;
                            }
                            error += dy2;
                        }
                    } else {
                        int error = dx2 - dy;
                        int offset = 0;
                        for (int count = 0; count <= dy; ++count) {
                            pixels[offset] = Gradient_Pixel(from, to, ratio);
                            Step_Ratio(ratio, step);
                            if (error > 0) {
                                ++offset;
                                error -= dy2;
                            }
                            error += dx2;
                            pixels = (unsigned short *)((char *)pixels + stride);
                        }
                    }
                }
                Unlock();
                return true;
            }
        }
    }
    return false;
}


// Draw a clipped 16-bit line between two screen points with depths (TibSun
// dsurface.cpp Draw_Depth_Shaded_Line): plots color where the line is in front
// of the depth ring (0x00839C6C) and the level ring (0x008315B4) is non-zero,
// darkened by level/128 unless the level is 127; write_depth stores the depth.
// The depths of clipped endpoints are interpolated by length.
bool DSurface::Draw_Depth_Shaded_Line(Rect const & cliprect, Point2D const & startpoint, Point2D const & endpoint, unsigned color, int start_depth, int end_depth, bool write_depth)
{
    Rect clipped_rect = cliprect.Intersect(Get_Rect());
    if (clipped_rect.Width == 0 && clipped_rect.Height == 0) {
        return(false);
    }
    Point2D start = Offset_Point(startpoint, clipped_rect);
    Point2D end = Offset_Point(endpoint, clipped_rect);
    int zfirst = start_depth;
    int zlast = end_depth;
    if (start.X > end.X) {
        Point2D temp = start;
        start = end;
        end = temp;
        zfirst = end_depth;
        zlast = start_depth;
    }
    Point2D unclipped = end;
    int px = start.X;
    int py = start.Y;
    if (!Clip_Line(start, end, clipped_rect)) {
        return(false);
    }
    int zend = zlast;
    int zstart = zfirst;
    if (end.X != unclipped.X || end.Y != unclipped.Y) {
        Point2D clipped_point;
        clipped_point.X = px - end.X;
        double clipped_dx = (double)clipped_point.X;
        clipped_point.Y = py - end.Y;
        int clipped = (int)(int)Fast_Sqrt((double)clipped_point.Y * (double)clipped_point.Y + clipped_dx * clipped_dx);
        Point2D full_point;
        full_point.X = px - unclipped.X;
        double full_dx = (double)full_point.X;
        full_point.Y = py - unclipped.Y;
        int amount = abs((int)(int)((double)clipped
            / (double)(int)(int)Fast_Sqrt((double)full_point.Y * (double)full_point.Y + full_dx * full_dx)
            * (double)(zfirst - zlast)));
        if (zfirst < zlast) {
            zend = zfirst + amount;
        } else {
            zend = zfirst - amount;
        }
    }
    if (start.X != px || start.Y != py) {
        Point2D clipped_point;
        clipped_point.X = start.X - unclipped.X;
        double clipped_dx = (double)clipped_point.X;
        clipped_point.Y = start.Y - unclipped.Y;
        int clipped = (int)(int)Fast_Sqrt((double)clipped_point.Y * (double)clipped_point.Y + clipped_dx * clipped_dx);
        Point2D full_point;
        full_point.X = px - unclipped.X;
        double full_dx = (double)full_point.X;
        full_point.Y = py - unclipped.Y;
        int amount = abs((int)(int)((double)clipped
            / (double)(int)(int)Fast_Sqrt((double)full_point.Y * (double)full_point.Y + full_dx * full_dx)
            * (double)(zfirst - zlast)));
        if (zlast > zfirst) {
            zstart = zlast - amount;
        } else {
            zstart = zlast + amount;
        }
    }
    Bytes_Per_Pixel();
    void * buffer = Lock(start);
    if (DepthBuffer == 0) {
        return(false);
    }
    Point2D point = start;
    point.Y -= DepthBuffer->Area.Y;
    unsigned short * zbuffer = DepthBuffer->Get_Buffer_Offset(point);
    point = start;
    point.Y -= AlphaBuffer->Area.Y;
    unsigned short * abuffer = AlphaBuffer->Get_Buffer_Offset(point);
    int start_y = start.Y;
    int zwrap = -1;
    unsigned short z = (unsigned short)(zstart + (short)DepthBuffer->Get_Scroll_Delta(start_y + clipped_rect.Y - DepthBuffer->Area.Y));
    int zwidth = DepthBuffer->Width;
    int red;
    int green;
    int blue;
    if (buffer != 0) {
        RGBClass rgb(
            (unsigned char)(((unsigned short)color >> unknown_shift_852fb8) << unknown_shift_852fbc),
            (unsigned char)(((unsigned short)color >> unknown_shift_852fc8) << unknown_shift_852fcc),
            (unsigned char)(((unsigned short)color >> unknown_shift_852fc0) << unknown_shift_852fc4));
        red = rgb.Red;
        green = rgb.Green;
        blue = rgb.Blue;
    } else {
        return(false);
    }
    int dy = end.Y - start.Y;
    int dx = end.X - start.X;
    int dz = zend - zstart;
    int pitch = Stride();
    if (dy < 0) {
        pitch = -pitch;
        zwidth = -zwidth;
        zwrap = 1;
    }
    int zstep = 1;
    int abs_dy = abs(dy);
    if (dz < 0) {
        zstep = -1;
    }
    int abs_dz = abs(dz);
    int dx2 = 2 * dx;
    int dy2 = 2 * abs_dy;
    int dz2 = 2 * abs_dz;
    if (abs_dz > dx && abs_dz > abs_dy) {
        int xdelta = dy2 - abs_dz;
        int ydelta = dx2 - abs_dz;
        if (abs_dz > 0) {
            int offset = 0;
            for (int i = abs_dz; i != 0; i--) {
                int v = *abuffer;
                if (z < *zbuffer && v != 0) {
                    if (v != 127) {
                        *(unsigned short *)((char *)buffer + offset) = (unsigned short)((((red * v) >> 7 >> unknown_shift_852fbc) << unknown_shift_852fb8) | (((green * v) >> 7 >> unknown_shift_852fcc) << unknown_shift_852fc8) | (((blue * v) >> 7 >> unknown_shift_852fc4) << unknown_shift_852fc0));
                    } else {
                        *(unsigned short *)((char *)buffer + offset) = (unsigned short)color;
                    }
                    if (write_depth) {
                        *zbuffer = z;
                    }
                }
                if (xdelta > 0) {
                    buffer = (unsigned char *)buffer + pitch;
                    xdelta -= dz2;
                    zbuffer += zwidth;
                    if (zwidth > 0) {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    } else {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
                    }
                    abuffer += zwidth;
                    if (zwidth > 0) {
                        abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
                    } else {
                        abuffer = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)abuffer);
                    }
                    z += zwrap;
                }
                if (ydelta > 0) {
                    zbuffer++;
                    offset += 2;
                    zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    abuffer++;
                    abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
                    ydelta -= dz2;
                }
                z += zstep;
                ydelta += dx2;
                xdelta += dy2;
            }
        }
    } else if (dx > abs_dy) {
        int ydelta = dy2 - dx;
        int i = 0;
        int zdelta = dz2 - dx;
        if (dx > 0) {
            for (; i < dx; i++) {
                int v = *abuffer;
                if (z < *zbuffer && v != 0) {
                    if (v != 127) {
                        *(unsigned short *)((char *)buffer + 2 * i) = (unsigned short)((((red * v) >> 7 >> unknown_shift_852fbc) << unknown_shift_852fb8) | (((green * v) >> 7 >> unknown_shift_852fcc) << unknown_shift_852fc8) | (((blue * v) >> 7 >> unknown_shift_852fc4) << unknown_shift_852fc0));
                    } else {
                        *(unsigned short *)((char *)buffer + 2 * i) = (unsigned short)color;
                    }
                    if (write_depth) {
                        *zbuffer = z;
                    }
                }
                if (ydelta > 0) {
                    buffer = (unsigned char *)buffer + pitch;
                    ydelta -= 2 * dx;
                    zbuffer += zwidth;
                    if (zwidth > 0) {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    } else {
                        zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
                    }
                    z += zwrap;
                    abuffer += zwidth;
                    if (zwidth > 0) {
                        abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
                    } else {
                        abuffer = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)abuffer);
                    }
                }
                if (zdelta > 0) {
                    z += zstep;
                    zdelta -= 2 * dx;
                }
                zbuffer++;
                ydelta += dy2;
                zdelta += dz2;
                zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                abuffer++;
                abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
            }
        }
    } else {
        int xdelta = dx2 - abs_dy;
        int zdelta = 2 * abs_dz - abs_dy;
        if (abs_dy > 0) {
            int i = abs_dy;
            int offset = 0;
            do {
                int v = *abuffer;
                if (z < *zbuffer && v != 0) {
                    if (v != 127) {
                        *(unsigned short *)((char *)buffer + offset) = (unsigned short)((((red * v) >> 7 >> unknown_shift_852fbc) << unknown_shift_852fb8) | (((green * v) >> 7 >> unknown_shift_852fcc) << unknown_shift_852fc8) | (((blue * v) >> 7 >> unknown_shift_852fc4) << unknown_shift_852fc0));
                    } else {
                        *(unsigned short *)((char *)buffer + offset) = (unsigned short)color;
                    }
                    if (write_depth) {
                        *zbuffer = z;
                    }
                }
                if (xdelta > 0) {
                    zbuffer++;
                    offset += 2;
                    zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                    abuffer++;
                    abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
                    xdelta -= dy2;
                }
                if (zdelta > 0) {
                    z += zstep;
                    zdelta -= dy2;
                }
                xdelta += dx2;
                zdelta += dz2;
                buffer = (unsigned char *)buffer + pitch;
                zbuffer += zwidth;
                if (zwidth > 0) {
                    zbuffer = (unsigned short *)DepthBuffer->Wrap_Overflow((unsigned int)zbuffer);
                } else {
                    zbuffer = (unsigned short *)DepthBuffer->Wrap_Underflow((unsigned int)zbuffer);
                }
                z += zwrap;
                abuffer += zwidth;
                if (zwidth > 0) {
                    abuffer = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)abuffer);
                } else {
                    abuffer = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)abuffer);
                }
                i--;
            } while (i != 0);
        }
    }
    Unlock();
    return(true);
}


// Draw_Dashed_Line limited by the depth buffer (TibSun dsurface.cpp
// Draw_Masked_Dashed_Line): plots the pattern's set entries only where the
// depth value is zero (empty_only) or only where it is not, and returns the
// updated pattern offset (16-bit surfaces only).
int DSurface::Draw_Dashed_Line_Depth_Masked(Point2D const &startpoint, Point2D const &endpoint, int color,
                                            bool const *pattern, int offset, bool empty_only)
{
    Point2D start = startpoint;
    Point2D end = endpoint;
    int pattern_step = 1;
    if (Bytes_Per_Pixel() != 2) return 0;
    if (start.X > end.X) {
        Point2D temp = start;
        start = end;
        end = temp;
        int dx = abs(start.X - end.X);
        int dy = abs(start.Y - end.Y) + 1;
        if (dx < dy) dx = dy;
        offset = (offset + dx) % 16;
        pattern_step = -1;
    }
    Point2D point = start;
    point.Y -= AlphaBuffer->Area.Y;
    unsigned short *depth = AlphaBuffer->Get_Buffer_Offset(point);
    int depth_stride = AlphaBuffer->Width;
    if (start.Y > end.Y) depth_stride = -depth_stride;
    unsigned short *pixels = (unsigned short *)Lock(start);
    if (pixels != 0) {
        if (empty_only) {
            if (start.Y == end.Y) {
                for (int i = 0; i <= end.X - start.X; i++, offset += pattern_step) {
                    if (offset < 0) offset += 16;
                    if (offset >= 16) offset -= 16;
                    if (pattern[offset] && *depth == 0) pixels[i] = (unsigned short)color;
                    if (depth_stride > 0) {
                        depth++;
                        depth = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)depth);
                    } else {
                        depth++;
                        depth = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)depth);
                    }
                }
            } else if (start.X == end.X) {
                int pitch = start.Y > end.Y ? -Stride() : Stride();
                int dy = abs(end.Y - start.Y);
                for (int i = 0; i <= dy; i++, offset += pattern_step) {
                    if (offset < 0) offset += 16;
                    if (offset >= 16) offset -= 16;
                    if (pattern[offset] && *depth == 0) *pixels = (unsigned short)color;
                    pixels = (unsigned short *)((unsigned char *)pixels + pitch);
                    if (depth_stride > 0) {
                        depth += depth_stride;
                        depth = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)depth);
                    } else {
                        depth += depth_stride;
                        depth = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)depth);
                    }
                }
            } else {
                int dx = end.X - start.X;
                int dy = end.Y - start.Y;
                int pitch = Stride();
                if (dy < 0) pitch = -pitch;
                dy = abs(dy);
                int dx2 = 2 * dx;
                int dy2 = 2 * dy;
                if (dx > dy) {
                    int delta = dy2 - dx;
                    for (int i = 0; i < dx; i++, offset += pattern_step) {
                        if (offset < 0) offset += 16;
                        if (offset >= 16) offset -= 16;
                        if (pattern[offset] && *depth == 0) pixels[i] = (unsigned short)color;
                        if (delta > 0) {
                            pixels = (unsigned short *)((unsigned char *)pixels + pitch);
                            depth += depth_stride;
                            depth = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)depth);
                            delta -= dx2;
                        }
                        delta += dy2;
                        if (depth_stride > 0) {
                            depth++;
                            depth = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)depth);
                        } else {
                            depth++;
                            depth = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)depth);
                        }
                    }
                } else {
                    int delta = dx2 - dy;
                    int k = 0;
                    for (int i = 0; i < dy; i++, offset += pattern_step) {
                        if (offset < 0) offset += 16;
                        if (offset >= 16) offset -= 16;
                        if (pattern[offset] && *depth == 0) pixels[k] = (unsigned short)color;
                        if (delta > 0) {
                            k++;
                            depth++;
                            depth = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)depth);
                            delta -= dy2;
                        }
                        delta += dx2;
                        pixels = (unsigned short *)((unsigned char *)pixels + pitch);
                        if (depth_stride > 0) {
                            depth += depth_stride;
                            depth = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)depth);
                        } else {
                            depth += depth_stride;
                            depth = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)depth);
                        }
                    }
                }
            }
        } else {
            if (start.Y == end.Y) {
                for (int i = 0; i <= end.X - start.X; i++, offset += pattern_step) {
                    if (offset < 0) offset += 16;
                    if (offset >= 16) offset -= 16;
                    if (pattern[offset] && *depth != 0) pixels[i] = (unsigned short)color;
                    if (depth_stride > 0) {
                        depth++;
                        depth = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)depth);
                    } else {
                        depth++;
                        depth = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)depth);
                    }
                }
            } else if (start.X == end.X) {
                int pitch = start.Y > end.Y ? -Stride() : Stride();
                int dy = abs(end.Y - start.Y);
                for (int i = 0; i <= dy; i++, offset += pattern_step) {
                    if (offset < 0) offset += 16;
                    if (offset >= 16) offset -= 16;
                    if (pattern[offset] && *depth != 0) *pixels = (unsigned short)color;
                    pixels = (unsigned short *)((unsigned char *)pixels + pitch);
                    if (depth_stride > 0) {
                        depth += depth_stride;
                        depth = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)depth);
                    } else {
                        depth += depth_stride;
                        depth = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)depth);
                    }
                }
            } else {
                int dx = end.X - start.X;
                int dy = end.Y - start.Y;
                int pitch = Stride();
                if (dy < 0) pitch = -pitch;
                dy = abs(dy);
                int dx2 = 2 * dx;
                int dy2 = 2 * dy;
                if (dx > dy) {
                    int delta = dy2 - dx;
                    for (int i = 0; i < dx; i++, offset += pattern_step) {
                        if (offset < 0) offset += 16;
                        if (offset >= 16) offset -= 16;
                        if (pattern[offset] && *depth != 0) pixels[i] = (unsigned short)color;
                        if (delta > 0) {
                            pixels = (unsigned short *)((unsigned char *)pixels + pitch);
                            depth += depth_stride;
                            depth = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)depth);
                            delta -= dx2;
                        }
                        delta += dy2;
                        if (depth_stride > 0) {
                            depth++;
                            depth = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)depth);
                        } else {
                            depth++;
                            depth = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)depth);
                        }
                    }
                } else {
                    int delta = dx2 - dy;
                    int k = 0;
                    for (int i = 0; i < dy; i++, offset += pattern_step) {
                        if (offset < 0) offset += 16;
                        if (offset >= 16) offset -= 16;
                        if (pattern[offset] && *depth != 0) pixels[k] = (unsigned short)color;
                        if (delta > 0) {
                            k++;
                            depth++;
                            depth = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)depth);
                            delta -= dy2;
                        }
                        delta += dx2;
                        pixels = (unsigned short *)((unsigned char *)pixels + pitch);
                        if (depth_stride > 0) {
                            depth += depth_stride;
                            depth = (unsigned short *)AlphaBuffer->Wrap_Overflow((unsigned int)depth);
                        } else {
                            depth += depth_stride;
                            depth = (unsigned short *)AlphaBuffer->Wrap_Underflow((unsigned int)depth);
                        }
                    }
                }
            }
        }
        Unlock();
        return offset;
    }
    return offset;
}


// Draw a clipped 16-bit line, plotting only where the depth buffer value is
// zero (empty_only) or only where it is not; the depth pointer walks the
// ABuffer ring alongside the pixel pointer.

bool DSurface::Draw_Line_Depth_Masked(Point2D const &startpoint, Point2D const &endpoint, int color, bool empty_only)
{
    if (Bytes_Per_Pixel() == 2) {
        Rect clip = Get_Rect();
        Point2D start = Offset_Point(startpoint, clip);
        Point2D end = Offset_Point(endpoint, clip);
        if (Clip_Line(start, end, clip)) {
            if (start.X > end.X) {
                Point2D temp = start;
                start = end;
                end = temp;
            }
            Point2D position = start;
            position.Y -= AlphaBuffer->Area.Y;
            unsigned short *depth = AlphaBuffer->Get_Buffer_Offset(position);
            int depth_stride = AlphaBuffer->Width;
            if (start.Y > end.Y) depth_stride = -depth_stride;
            unsigned short *pixels = (unsigned short *)Lock(start);
            if (pixels) {
            if (empty_only) {
                if (start.Y == end.Y) {
                    for (int index = 0; index <= end.X - start.X; ++index) {
                        if (*depth == 0) pixels[index] = (unsigned short)color;
                        AlphaBuffer->Advance_Pointer(depth, 1, depth_stride);
                    }
                } else if (start.X == end.X) {
                    int stride;
                    if (start.Y > end.Y) stride = -Stride();
                    else stride = Stride();
                    int height = abs(end.Y - start.Y);
                    for (int index = 0; index <= height; ++index) {
                        if (*depth == 0) *pixels = (unsigned short)color;
                        AlphaBuffer->Advance_Pointer(depth, depth_stride, depth_stride);
                        pixels = (unsigned short *)((char *)pixels + stride);
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
                            if (*depth == 0) pixels[index] = (unsigned short)color;
                            if (error > 0) {
                                pixels = (unsigned short *)((char *)pixels + stride);
                                depth = AlphaBuffer->Wrap_Pointer(depth + depth_stride);
                                error -= dx2;
                            }
                            AlphaBuffer->Advance_Pointer(depth, 1, depth_stride);
                            error += dy2;
                        }
                    } else {
                        int error = dx2 - dy;
                        int offset = 0;
                        for (int count = dy; count > 0; --count) {
                            if (*depth == 0) pixels[offset] = (unsigned short)color;
                            if (error > 0) {
                                error -= dy2;
                                ++offset;
                                depth = AlphaBuffer->Wrap_Pointer(depth + 1);
                            }
                            error += dx2;
                            pixels = (unsigned short *)((char *)pixels + stride);
                            AlphaBuffer->Advance_Pointer(depth, depth_stride, depth_stride);
                        }
                    }
                }
            } else {
                if (start.Y == end.Y) {
                    for (int index = 0; index <= end.X - start.X; ++index) {
                        if (*depth != 0) pixels[index] = (unsigned short)color;
                        AlphaBuffer->Advance_Pointer(depth, 1, depth_stride);
                    }
                } else if (start.X == end.X) {
                    int stride;
                    if (start.Y > end.Y) stride = -Stride();
                    else stride = Stride();
                    int height = abs(end.Y - start.Y);
                    for (int index = 0; index <= height; ++index) {
                        if (*depth != 0) *pixels = (unsigned short)color;
                        AlphaBuffer->Advance_Pointer(depth, depth_stride, depth_stride);
                        pixels = (unsigned short *)((char *)pixels + stride);
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
                            if (*depth != 0) pixels[index] = (unsigned short)color;
                            if (error > 0) {
                                pixels = (unsigned short *)((char *)pixels + stride);
                                depth = AlphaBuffer->Wrap_Pointer(depth + depth_stride);
                                error -= dx2;
                            }
                            AlphaBuffer->Advance_Pointer(depth, 1, depth_stride);
                            error += dy2;
                        }
                    } else {
                        int error = dx2 - dy;
                        int offset = 0;
                        for (int count = dy; count > 0; --count) {
                            if (*depth != 0) pixels[offset] = (unsigned short)color;
                            if (error > 0) {
                                error -= dy2;
                                ++offset;
                                depth = AlphaBuffer->Wrap_Pointer(depth + 1);
                            }
                            error += dx2;
                            pixels = (unsigned short *)((char *)pixels + stride);
                            AlphaBuffer->Advance_Pointer(depth, depth_stride, depth_stride);
                        }
                    }
                }
            }
                Unlock();
                return true;
            }
        }
    }
    return false;
}


// HRESULT Init_Direct3D() (0x004B1340)
// Creates the Direct3D device on the composing surface's DirectDraw surface:
// IDirect3D and IDirect3D2 from the DirectDraw object, a HAL device, and a
// viewport covering unknown_8523e0 x unknown_8523e4 that becomes the
// device's current viewport. Returns the first failing HRESULT (the
// viewport is released again when the device rejects it). The name is a
// hypothesis.

HRESULT Init_Direct3D()
{
    IDirectDrawSurface *surface = ((DSurface *)unknown_surface_839964)->Get_DD_Surface();
    HRESULT result = DirectDrawObject->QueryInterface(&IID_IDirect3D, (void **)&unknown_direct3d_852fe8);
    if (result != DD_OK) {
        return result;
    }
    result = unknown_direct3d_852fe8->QueryInterface(IID_IDirect3D2, (void **)&unknown_direct3d2_852fdc);
    if (result != DD_OK) {
        return result;
    }
    result = unknown_direct3d2_852fdc->CreateDevice(IID_IDirect3DHALDevice, surface, &unknown_device_852fe0);
    if (result != DD_OK) {
        return result;
    }
    result = unknown_direct3d2_852fdc->CreateViewport(&unknown_viewport_852fe4, 0);
    if (result != DD_OK) {
        return result;
    }
    result = unknown_device_852fe0->AddViewport(unknown_viewport_852fe4);
    if (result != DD_OK) {
        unknown_viewport_852fe4->Release();
        unknown_viewport_852fe4 = 0;
        return result;
    }
    D3DVIEWPORT2 viewport;
    memset(&viewport, 0, sizeof(viewport));
    viewport.dwSize = sizeof(viewport);
    viewport.dwWidth = unknown_8523e0;
    viewport.dwHeight = unknown_8523e4;
    viewport.dvClipX = -1.0f;
    viewport.dvClipWidth = 2.0f;
    viewport.dvClipY = 1.0f;
    viewport.dvClipHeight = 2.0f;
    viewport.dvMaxZ = 1.0f;
    unknown_viewport_852fe4->SetViewport2(&viewport);
    return unknown_device_852fe0->SetCurrentViewport(unknown_viewport_852fe4);
}


// void Release_Direct3D() (0x004B1470)
// Releases the viewport, the device and the IDirect3D2 interface created by
// Init_Direct3D (0x004B1340). The name is a hypothesis; the IDirect3D
// interface (0x00852FE8) is not released here.

void Release_Direct3D()
{
    if (unknown_viewport_852fe4 != 0) {
        unknown_viewport_852fe4->Release();
        unknown_viewport_852fe4 = 0;
    }
    if (unknown_device_852fe0 != 0) {
        unknown_device_852fe0->Release();
        unknown_device_852fe0 = 0;
    }
    if (unknown_direct3d2_852fdc != 0) {
        unknown_direct3d2_852fdc->Release();
        unknown_direct3d2_852fdc = 0;
    }
}


// bool DSurface::Copy_From(Surface &source, bool trans, bool unknown) (0x004B14C0, slot 1)
// Inline in shared/dsurface.hpp: forwards to XSurface's whole-surface copy.
// Emitted with the DSurface vtable at the end of the unit.


// bool DSurface::Is_Direct_Draw() (0x004B14E0, slot 33)
// Inline in shared/dsurface.hpp: a DirectDraw surface (true).


// DSurface scalar deleting destructor (0x004B14F0, slot 0)
// Compiler generated with the DSurface vtable; the destructor is expanded
// inline.
