// bfiofile2.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/bfiofile2.json carries the addresses and the rows that
// stay in their own files.


#include "shared/movie.hpp"
#include "shared/dsurface.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/keyboard.hpp"
#include "shared/win32_api.hpp"


// BinkClass::BinkClass(char const *filename) (0x0042FE70)
// Opens the movie for the default target (chosen by Open).

BinkClass::BinkClass(char const *filename) :
    unknown_00(false),
    Handle(0),
    Target(0),
    Companion(0),
    File(INVALID_HANDLE_VALUE),
    unknown_2c(true),
    unknown_2d(false)
{
    Open(filename);
}


// BinkClass::BinkClass(char const *filename, Surface *surface) (0x0042FEA0)
// Opens the movie to play onto `surface`.

BinkClass::BinkClass(char const *filename, Surface *surface) :
    unknown_00(false),
    Handle(0),
    Target(surface),
    Companion(0),
    File(INVALID_HANDLE_VALUE),
    unknown_2c(true),
    unknown_2d(false)
{
    Open(filename);
}


// BinkClass::~BinkClass() (0x0042FEE0)

BinkClass::~BinkClass()
{
    Close();
}


// bool BinkClass::Open(char const *filename) (0x0042FF30)
// Opens the movie as a file or from inside a mixfile, sets its volume and
// frame interval, picks the target surface (the screen surface centred in
// the window client area unless a companion is present), clips the movie
// rectangle to the target and records the screen's Bink surface type.


bool BinkClass::Open(char const *filename)
{
    Close();
    if (unknown_check_406f30()) {
        BinkSetSoundSystem((void *)BinkOpenDirectSound, (unsigned long)unknown_directsound_40a6c0());
    }
    if (RawFileClass(filename).Is_Available() == true) {
        Handle = BinkOpen(filename, 0);
    } else {
        long offset = 0;
        MixFileClass *mixfile = 0;
        if (MixFileClass::Offset(filename, 0, &mixfile, &offset, 0) == true) {
            File = CreateFileA(mixfile->Filename, 0x80000000, 3, 0, 3, 0x8000080, 0);
            if (File != INVALID_HANDLE_VALUE) {
                SetFilePointer(File, offset, 0, 0);
                Handle = BinkOpen((char const *)File, 0x800000);
            }
        }
    }
    if (Handle == 0) {
        Debug_Printf("Bink Error: %s\n", BinkGetError());
        return false;
    }
    *(int *)&unknown_volume_84e9a8 = *(int *)&unknown_volume_a40b48;
    BinkSetVolume(Handle, (int)(unknown_volume_84e9a8 * 32768.0f));
    unknown_24 = 60 / (Handle->FrameRate / Handle->FrameRateDiv);
    Companion = unknown_create_companion_69b190(filename);
    Rect movie(0, 0, Handle->Width, Handle->Height);
    if (Target == 0) {
        if (Companion != 0 && *(int *)Companion != 0) {
            Target = unknown_surface_839954;
            movie.X = (Target->Get_Width() - movie.Width) / 2;
            movie.Y = (Target->Get_Height() - movie.Height) / 2;
        } else {
            Target = unknown_surface_839950;
            RECT client;
            GetClientRect(MainWindow, &client);
            int clientwidth = client.right - client.left;
            int clientheight = client.bottom - client.top;
            movie.X = (clientwidth - movie.Width) / 2;
            movie.Y = (clientheight - movie.Height) / 2;
        }
    }
    Area = Target->Get_Rect().Intersect(movie);
    SurfaceType = BinkDDSurfaceType(((DSurface *)unknown_surface_839950)->Get_DD_Surface());
    return true;
}


// void BinkClass::Close() (0x00430230)
// Frees the companion, closes the movie and the file handle.

void BinkClass::Close()
{
    if (Companion != 0) {
        delete Companion;
        Companion = 0;
    }
    if (Handle != 0) {
        BinkClose(Handle);
        Handle = 0;
    }
    if (File != INVALID_HANDLE_VALUE) {
        CloseHandle(File);
        File = INVALID_HANDLE_VALUE;
    }
}


// void BinkClass::Set_Position(int x, int y) (0x00430280)
// Places the movie at x, y, clipped to the target surface.

void BinkClass::Set_Position(int x, int y)
{
    Area = Intersect(Target->Get_Rect(), Rect(x, y, Handle->Width, Handle->Height), 0, 0);
}


// void BinkClass::Goto_Frame(int frame) (0x004303A0): BinkGoto(frame, 1).

void BinkClass::Goto_Frame(int frame)
{
    BinkGoto(Handle, frame, 1);
}


// int BinkClass::Frame_Time() (0x004303C0): milliseconds per frame.

int BinkClass::Frame_Time()
{
    return 1000 / (Handle->FrameRate / Handle->FrameRateDiv);
}


// void BinkClass::Set_Subtitle_Callback(int unknown) (0x004303E0): forwards to the companion.

void BinkClass::Set_Subtitle_Callback(int unknown)
{
    if (Companion != 0) {
        Companion->SetCallback_0069b4f0(unknown);
    }
}


// void BinkClass::Pause(bool pause) (0x00430400): BinkPause.

void BinkClass::Pause(bool pause)
{
    BinkPause(Handle, pause);
}


// bool BinkClass::Is_Finished() (0x00430420): the movie is past its last frame.

bool BinkClass::Is_Finished()
{
    return Handle->FrameNum >= Handle->Frames;
}


// void BinkClass::Play() (0x00430430)
// Plays the movie to its last frame: window messages are dispatched first,
// otherwise the due frames are drawn (at the movie area of the target; a
// movie drawn elsewhere is then copied to the screen surface at the window
// client area). Escape stops it unless unknown_00 is set.

void BinkClass::Play()
{
    if (Handle == 0) {
        Debug_Printf("Bink Error: %s\n", BinkGetError());
        return;
    }
    WWKeyboard->Clear();
    unknown_2c = true;
    unknown_2d = false;
    while (Handle->FrameNum < Handle->Frames) {
        MSG message;
        unknown_message_count_a52298++;
        if (unknown_peek_message_522ed0(&message, 0, 0, 0, 0)) {
            if (!unknown_get_message_522e90(&message, 0, 0, 0)) {
                break;
            }
            TranslateMessage(&message);
            DispatchMessageA(&message);
        } else {
            if (!unknown_2c) {
                Sleep(33);
            }
            if (Target == unknown_surface_839950) {
                RECT client;
                GetClientRect(MainWindow, &client);
                ClientToScreen(MainWindow, (POINT *)&client);
                Draw_Frame(Target, Area.X + client.left, Area.Y + client.top);
            } else if (Draw_Frame(Target, Area.X, Area.Y)) {
                RECT client;
                GetClientRect(MainWindow, &client);
                ClientToScreen(MainWindow, (POINT *)&client);
                Rect dest = Area;
                dest.X += client.left;
                dest.Y += client.top;
                unknown_surface_839950->Copy_From(dest, *Target, Area, false, true);
            }
            if (WWKeyboard->Check() && !unknown_00 && WWKeyboard->Get() == 0x81B) {
                break;
            }
        }
    }
    WWKeyboard->Clear();
}


// bool BinkClass::Draw_Frame(Surface *surface, int x, int y) (0x00430600)
// Follows the volume setting, pauses or resumes the movie with the
// application focus, then decodes and copies every due frame into `surface`
// at x, y; false when no frame was due.

extern bool unknown_flag_a40d28;                // 0x00A40D28 application active (focus)

bool BinkClass::Draw_Frame(Surface *surface, int x, int y)
{
    if (unknown_volume_84e9a8 != unknown_volume_a40b48) {
        // Copied as a bit pattern: the original moves it through eax and reads the copy back.
        *(int *)&unknown_volume_84e9a8 = *(int *)&unknown_volume_a40b48;
        BinkSetVolume(Handle, (int)(unknown_volume_84e9a8 * 32768.0f));
    }
    if (!unknown_flag_a40d28) {
        if (unknown_2c == true) {
            Debug_Printf("Pause bink movie\n");
            unknown_2c = false;
            BinkPause(Handle, 1);
        }
    } else if (!unknown_2c) {
        Debug_Printf("Resume bink movie\n");
        unknown_2c = true;
        BinkPause(Handle, 0);
        if (Target == unknown_surface_839950) {
            RECT client;
            GetClientRect(MainWindow, &client);
            ClientToScreen(MainWindow, (POINT *)&client);
            Copy_Frame(Target, Area.X + client.left, Area.Y + client.top, true);
        } else {
            Copy_Frame(Target, Area.X, Area.Y, true);
        }
    }
    if (!unknown_2d) {
        bool due = BinkWait(Handle) == 0;
        if (!due) {
            return due;
        }
    }
    do {
        BinkDoFrame(Handle);
        unknown_2d = false;
        bool special = false;
        if (Companion != 0) {
            special = Companion->Process_0069b420(Handle->FrameNum * unknown_24);
            if (special) {
                Erase_Subtitle(surface);
            }
        }
        void *buffer = surface->Lock();
        if (buffer != 0) {
            int pitch = surface->Stride();
            int height = surface->Get_Height();
            unsigned flags = SurfaceType;
            if (special) {
                flags |= 0x80000000;
            }
            BinkCopyToBuffer(Handle, buffer, pitch, height, x, y, flags);
            surface->Unlock();
        }
        Draw_Subtitle(surface);
        BinkNextFrame(Handle);
    } while (BinkWait(Handle) == 0);
    return true;
}


// void BinkClass::Check_Frame_Ready() (0x004307E0): marks the next frame due (BinkWait 0).

void BinkClass::Check_Frame_Ready()
{
    if (BinkWait(Handle) == 0) {
        unknown_2d = true;
    }
}


// bool BinkClass::Render_Frame() (0x00430800): draws the frame onto the target.

bool BinkClass::Render_Frame()
{
    return Draw_Frame(Target, Area.X, Area.Y);
}


// void BinkClass::Redraw() (0x00430820)
// Copies the current frame into the target surface again at the movie area
// (offset by the window client area on the screen surface).

void BinkClass::Redraw()
{
    if (Target == unknown_surface_839950) {
        RECT client;
        GetClientRect(MainWindow, &client);
        ClientToScreen(MainWindow, (POINT *)&client);
        int x = Area.X + client.left;
        int y = Area.Y + client.top;
        Redraw_Frame(Target, x, y);
    } else {
        Redraw_Frame(Target, Area.X, Area.Y);
    }
}


// void BinkClass::Redraw_Frame(Surface *surface, int x, int y) (0x00430940)
// Copies the current frame into `surface` at x, y again (with the companion
// areas cleared first and drawn after).

void BinkClass::Redraw_Frame(Surface *surface, int x, int y)
{
    Erase_Subtitle(surface);
    void *buffer = surface->Lock();
    if (buffer != 0) {
        int pitch = surface->Stride();
        int height = surface->Get_Height();
        BinkCopyToBuffer(Handle, buffer, pitch, height, x, y, SurfaceType | 0x80000000);
        surface->Unlock();
    }
    Draw_Subtitle(surface);
}


// void BinkClass::Copy_Frame(Surface *surface, int x, int y, bool unknown4) (0x004309B0)
// Copies the current frame into `surface` at x, y; with unknown4 the
// companion's clear area is filled first and the copy uses Bink's 0x80000000 flag.

#pragma auto_inline(off)
void BinkClass::Copy_Frame(Surface *surface, int x, int y, bool unknown4)
{
    if (unknown4) {
        Erase_Subtitle(surface);
    }
    void *buffer = surface->Lock();
    if (buffer != 0) {
        int pitch = surface->Stride();
        int height = surface->Get_Height();
        unsigned flags = SurfaceType;
        if (unknown4) {
            flags |= 0x80000000;
        }
        BinkCopyToBuffer(Handle, buffer, pitch, height, x, y, flags);
        surface->Unlock();
    }
    Draw_Subtitle(surface);
}
#pragma auto_inline(on)


// void BinkClass::Draw_Subtitle(Surface *surface) (0x00430A30)
// Draws the companion's picture over the movie area of `surface`.

#pragma auto_inline(off)
void BinkClass::Draw_Subtitle(Surface *surface)
{
    if (Companion != 0 && Companion->unknown_00 != 0) {
        Rect &area = Companion->unknown_14;
        if (area.Is_Valid()) {
            Surface *source = Companion->unknown_10;
            Rect full = source->Get_Rect();
            Rect dest = area;
            dest.X += (Area.Width - full.Width) / 2;
            dest.Y += Area.Height - full.Height;
            dest.X += Area.X;
            dest.Y += Area.Y;
            surface->Copy_From(dest, *source, area, true, true);
        }
    }
}
#pragma auto_inline(on)


// void BinkClass::Erase_Subtitle(Surface *surface) (0x00430AF0)
// Clears the companion's clear area (colour 0) within the movie area.

#pragma auto_inline(off)
void BinkClass::Erase_Subtitle(Surface *surface)
{
    if (Companion != 0 && Companion->unknown_00 != 0) {
        Rect area = Companion->unknown_24;
        if (area.Is_Valid()) {
            Rect full = Companion->unknown_10->Get_Rect();
            area.X += (Area.Width - full.Width) / 2;
            area.Y += Area.Height - full.Height;
            area.X += Area.X;
            area.Y += Area.Y;
            surface->Fill_Rect(area, 0);
        }
    }
}
#pragma auto_inline(on)


// 0x00430ba0: function not yet identified
//
// Body: 11 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: int __fastcall FUN_00430ba0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00430ba0 {

int __fastcall FUN_00430ba0(int object_address)
{
    return *(int *)(*(int *)(object_address + 4) + 0xc) * *(int *)(object_address + 0x24);
}

}  // namespace seed_00430ba0


// void BinkClass::Update_Focus() (0x00430BB0)
// Pauses the movie when the application loses the focus and resumes it (and
// redraws the current frame) when it gets it back. No callers.

extern bool unknown_flag_a40d28;                // 0x00A40D28 application active (focus)

void BinkClass::Update_Focus()
{
    if (!unknown_flag_a40d28) {
        if (unknown_2c == true) {
            Debug_Printf("Pause bink movie\n");
            unknown_2c = false;
            BinkPause(Handle, 1);
        }
    } else if (!unknown_2c) {
        Debug_Printf("Resume bink movie\n");
        unknown_2c = true;
        BinkPause(Handle, 0);
        Redraw();
    }
}
