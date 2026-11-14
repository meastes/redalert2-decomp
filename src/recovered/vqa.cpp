// vqa.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/vqa.json carries the addresses and the rows that
// stay in their own files.


// Dynamic initializer of unknown_empty_rect (0x0071C320, _$E), the file-local
// empty Rect from shared/shape_file.hpp (0x00AF8F98 in this unit); the first
// of the unit's header-static initializers.
#include "shared/movie.hpp"
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"


// Dynamic initializer of unknown_geometry_diagonal (0x0071C340, _$E) from
// shared/unknown_geometry.hpp; emitted by the include in 0x0071C320's draft.


// Dynamic initializer of unknown_geometry_b (0x0071C370, _$E) from
// shared/unknown_geometry.hpp; emitted by the include in 0x0071C320's draft.


// Dynamic initializer of unknown_geometry_c (0x0071C390, _$E) from
// shared/unknown_geometry.hpp; emitted by the include in 0x0071C320's draft.


// Dynamic initializer of unknown_geometry_angle45 (0x0071C3B0, _$E) from
// shared/unknown_geometry.hpp; emitted by the include in 0x0071C320's draft.


// Dynamic initializer of unknown_geometry_angle60 (0x0071C3D0, _$E) from
// shared/unknown_geometry.hpp; emitted by the include in 0x0071C320's draft.


// Dynamic initializer of unknown_geometry_angle90 (0x0071C3F0, _$E) from
// shared/unknown_geometry.hpp; emitted by the include in 0x0071C320's draft.


// Dynamic initializer of unknown_geometry_g (0x0071C410, _$E) from
// shared/unknown_geometry.hpp; emitted by the include in 0x0071C320's draft.


// Dynamic initializer of unknown_geometry_h (0x0071C450, _$E) from
// shared/unknown_geometry.hpp; emitted by the include in 0x0071C320's draft.


// Dynamic initializer of unknown_geometry_i (0x0071C480, _$E) from
// shared/unknown_geometry.hpp; emitted by the include in 0x0071C320's draft.


// Dynamic initializer of unknown_geometry_j (0x0071C4B0, _$E) from
// shared/unknown_geometry.hpp; emitted by the include in 0x0071C320's draft.


// Dynamic initializer of unknown_geometry_k (0x0071C4E0, _$E) from
// shared/unknown_geometry.hpp; emitted by the include in 0x0071C320's draft.


// Dynamic initializer of unknown_geometry_l (0x0071C4F0, _$E) from
// shared/unknown_geometry.hpp; emitted by the include in 0x0071C320's draft.


// Dynamic initializer of the VQA unit's movie handle list (0x0071C510, _$E):
// DynamicVectorClass<MovieHandle *> at 0x00AF8FD8, destroyed at exit
// (0x0071C550). Name of the list is a hypothesis.
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

DynamicVectorClass<MovieHandle *> unknown_movie_handles_af8fd8;


// _$E (0x0071C550): atexit thunk destroying the movie handle list; the
// compiler emits it from the definition in 0x0071C510's draft.


// bool VQA_Unknown_0071C590() (0x0071C590)
// Handles one pending window message (counted in 0x00A52298); false when
// the message loop has to end (WM_QUIT).

bool VQA_Unknown_0071C590()
{
    MSG message;
    ++unknown_message_count_a52298;
    if (unknown_peek_message_522ed0(&message, 0, 0, 0, 0)) {
        if (unknown_get_message_522e90(&message, 0, 0, 0)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        } else {
            return false;
        }
    }
    return true;
}


// VQAClass::VQAClass(char const *filename, int flags, int (*callback538)(), void (*callback53c)(),
// void (*callback540)(), int unknown6, int unknown7) (0x0071C5F0): the library's default configuration, then
// the callbacks and options from the VQA option bits and flags (1: drawer 0x0071D640, 2 and 4: option bits).
#include "shared/crt_string.hpp"

unsigned long __cdecl Unknown_004122C0(VQAHandle *vqa);
unsigned long __cdecl Unknown_00411420(VQAHandle *vqa);
long __cdecl Stream_Audio_Handler(VQAHandle *vqa, long action, void *buffer, long nbytes);   // 0x004115A0 (shared/ahandle.hpp)

VQAClass::VQAClass(char const *filename, int flags, int (*callback538)(), void (*callback53c)(),
                   void (*callback540)(), int unknown6, int unknown7)
{
    char unknown;
    VQA_Unknown_00777850(&Config);
    Flags = flags;
    Config.Owner = this;
    Config.ImageBuf = &unknown;
    Config.DrawFlags = 0;
    if (!VQA_Unknown_0071DBA0(15)) {
        Config.DrawFlags |= 1;
    }
    if (!VQA_Unknown_0071DBA0(1)) {
        Config.DrawFlags |= 4;
    }
    if (VQA_Unknown_0071DBA0(10) == true) {
        Config.DrawFlags |= 0x40;
    }
    if (VQA_Unknown_0071DBA0(12) == true) {
        Config.DrawFlags |= 0x80;
    }
    Config.OptionFlags |= 8;
    if (Flags & 2) {
        Config.OptionFlags |= 0x20;
        if (Flags & 4) {
            Config.OptionFlags |= 0x100;
        }
    } else {
        Config.OptionFlags &= ~0x20;
    }
    if (VQA_Unknown_0071DBA0(17) == true) {
        Config.OptionFlags |= 4;
    } else {
        Config.OptionFlags &= ~4;
    }
    if (VQA_Unknown_0071DBA0(3) == true) {
        Config.OptionFlags &= ~1;
    } else {
        Config.OptionFlags |= 1;
    }
    Config.ImageWidth = -1;
    Config.ImageHeight = -1;
    Config.X1 = 0;
    Config.Y1 = 0;
    Config.DrawerCallback = 0;
    Config.FrameRate = unknown6;
    Config.DrawRate = unknown7;
    if (VQA_Unknown_0071DBA0(3) == true) {
        Config.NumFrameBufs = 1;
    } else {
        Config.NumFrameBufs = 14;
    }
    Config.NumCBBufs = -1;
    if (Flags & 1) {
        Config.StreamHandler = VQA_Unknown_0071D640;
    } else {
        Config.StreamHandler = VQA_Unknown_0071D490;
    }
    Config.MemoryHandler = VQA_Unknown_0071D680;
    Config.EventHandler = VQA_Unknown_0071D6E0;
    if (VQA_Unknown_0071DBA0(4) == true) {
        Config.TimerCallback = Unknown_004122C0;
        Config.RefreshRate = 60;
    } else {
        Config.TimerCallback = Unknown_00411420;
        Config.RefreshRate = 60;
        Config.LatencyAdjustment = unknown_int_a40b54 & 0xFFFF;
    }
    Config.AudioHandler = (VQACallback)Stream_Audio_Handler;
    Config.HMIBufSize = 0x2000;
    CurrentFrame = 0;
    unknown_544 = 0;
    FrameCount = 0;
    strcpy(Filename, filename);
    Width = 0;
    Height = 0;
    unknown_518 = 0;
    unknown_51c = 0;
    unknown_508 = -1;
    PaletteChanged = false;
    Callback538 = callback538;
    Callback53C = callback53c;
    Callback540 = callback540;
    FileOpen = false;
    Opened = false;
    unknown_548 = false;
    unknown_549 = false;
    Companion = 0;
}


// VQAClass::~VQAClass() (0x0071C8A0)

VQAClass::~VQAClass()
{
    Unknown_0070B7D0();
}


// int VQAClass::Cache_VQA(unsigned size) (0x0071C8D0)
// Loads the movie file into Buffer in pieces of `size` bytes (0: the rest);
// returns -1 while more remains, -2 when complete, -3/-4/-5 on errors.

extern "C" void *__cdecl malloc(unsigned size);

int VQAClass::Cache_VQA(unsigned size)
{
    if (!FileOpen) {
        FileOpen = File.Open(Filename, 1) != 0;
        if (!FileOpen) {
            return -4;
        }
        FileSize = File.Size();
        unknown_530 = 0;
        Position = 0;
        Buffer = (unsigned char *)malloc(FileSize);
        if (Buffer == 0) {
            File.Close();
            FileOpen = false;
            return -3;
        }
    }
    if (size == 0) {
        size = FileSize - Position;
    } else if (Position + size > FileSize) {
        size = FileSize - Position;
    }
    if ((unsigned)File.Read(Buffer + Position, size) != size) {
        File.Close();
        FileOpen = false;
        unknown_530 = 0;
        Position = 0;
        return -5;
    }
    Position += size;
    if (Position >= FileSize) {
        File.Close();
        FileOpen = false;
        return -2;
    }
    return -1;
}


// VQAClass stream callback over the movie file held in memory (Buffer,
// FileSize bytes, read position unknown_530); 0 on success, 1 on failure.
// The read clips at FileSize; seek origin 0 = set, 1 = current, 2 = end.
#include "shared/crt_memory.hpp"

int VQAClass::CacheHandler(int request, int unknown2, int unknown3)
{
    int rc = 1;
    switch (request) {
    case 1:
    case 2:
    case 3:
    case 4:
        rc = 0;
        break;
    case 5:
        if (unknown_530 + unknown3 > FileSize) {
            rc = 1;
            return rc;
        } else {
            memcpy((void *)unknown2, Buffer + unknown_530, unknown3);
            unknown_530 += unknown3;
            rc = 0;
        }
        break;
    case 7:
        switch (unknown2) {
        case 1:
            unknown_530 += unknown3;
            rc = 0;
            break;
        case 0:
            unknown_530 = unknown3;
            rc = 0;
            break;
        case 2:
            unknown_530 = FileSize - unknown3 - 1;
            rc = 0;
            break;
        default:
            rc = 1;
            break;
        }
        break;
    default:
        rc = 1;
        break;
    }
    return rc;
}


// void VQAClass::Handle_Palette_Event(unsigned char const *palette) (0x0071CAF0)
// Copies the 768-byte palette and marks it changed.

void VQAClass::Handle_Palette_Event(unsigned char const *palette)
{
    memcpy(Palette, palette, sizeof(Palette));
    PaletteChanged = true;
}


// void VQAClass::Set_DirectDraw_Palette() (0x0071CB20): makes the movie palette current.
#include "shared/palette.hpp"

void VQAClass::Set_DirectDraw_Palette()
{
    ((PaletteClass *)Palette)->Set();
}


// bool VQAClass::Open_And_Load_Buffers() (0x0071CB30)
// Opens the movie file and records its frame count and size.

bool VQAClass::Open_And_Load_Buffers()
{
    if (VQA_Unknown_0077B7A0(Filename, &Config, &Handle) != -1) {
        Opened = false;
        return false;
    }
    FrameCount = Handle->Frames;
    unknown_504 = Handle->unknown_0C;
    Width = Handle->Width;
    Height = Handle->Height;
    Opened = true;
    return true;
}


// int VQAClass::Get_VQA_Version() (0x0071CBA0): the handle's first word, or -1.

int VQAClass::Get_VQA_Version()
{
    if (Handle != 0) {
        return Handle->unknown_00;
    }
    return -1;
}


// int VQAClass::Set_VQA_Volume(int value) (0x0071CBC0): sets the configuration volume (-1 keeps it); returns the old one.

int VQAClass::Set_VQA_Volume(int value)
{
    int old = Config.Volume;
    if (value != -1) {
        Config.Volume = value;
    }
    return old;
}


// bool VQAClass::Set_Loop(int unknown1, int unknown2) (0x0071CBE0)

bool VQAClass::Set_Loop(int unknown1, int unknown2)
{
    if (unknown2 < 0) {
        unknown2 = -1;
    }
    return VQA_Unknown_0077C640(Handle, unknown1, unknown2, 1) == -1 ? true : false;
}


// bool VQAClass::Set_Loop(int unknown1, int unknown2, int unknown3) (0x0071CC10)
// Frame range (clamped to the movie) and count.

bool VQAClass::Set_Loop(int unknown1, int unknown2, int unknown3)
{
    if (unknown1 < 0 || unknown1 >= FrameCount) {
        unknown1 = 0;
    }
    if (unknown2 < 0 || unknown2 >= FrameCount) {
        unknown2 = FrameCount - 1;
    }
    if (unknown3 < 0) {
        unknown3 = -1;
    }
    return VQA_Unknown_0077C6C0(Handle, unknown1, unknown2, unknown3, 0) == -1 ? true : false;
}


// void VQAClass::Seek_To_Frame(int frame) (0x0071CC60): seeks to the frame
// unless it is already current; the frame is kept when the seek (0x0077C2E0)
// does not fail with -6.

void VQAClass::Seek_To_Frame(int frame)
{
    if (frame != CurrentFrame) {
        if (VQA_Unknown_0077C2E0(Handle, frame, 0) != -6) {
            CurrentFrame = frame;
        }
    }
}


// bool VQAClass::Seek_To_Stored_Frame() (0x0071CC90): seeks back to the saved frame.

bool VQAClass::Seek_To_Stored_Frame()
{
    if (Opened) {
        if (VQA_Unknown_0077C2E0(Handle, unknown_544, 0) != -6) {
            CurrentFrame = unknown_544;
        } else {
            return false;
        }
    }
    return true;
}


// int VQAClass::Play_VQA(int frame, bool unknown) (0x0071CCD0): plays the movie until it ends (to
// frame; out of range: the last), pumping window messages and sleeping while the application is inactive;
// returns 1 when stopped by Escape (keys are not read when unknown is set).
#include "shared/debug_printf.hpp"
#include "shared/keyboard.hpp"
#include "shared/unknown_globals.hpp"

int VQAClass::Play_VQA(int frame, bool unknown)
{
    bool done = false;
    bool sleeping = false;
    if (frame < 0 || frame >= FrameCount) {
        frame = FrameCount - 1;
    }
    VQA_Unknown_0077C8A0(Handle, frame);
    if (unknown_508 != -1) {
        int unknown2;
        int unknown3;
        int state;
        VQA_Unknown_0077C920(Handle, &unknown3, &unknown2, &state);
        if (state == 1) {
            VQA_Unknown_0070B690(unknown_508);
            if (VQA_Unknown_0071DBA0(11)) {
                VQA_Unknown_0077C440(Handle, VQA_Unknown_0077FE89, VQA_Unknown_0070BBC0);
            } else {
                VQA_Unknown_0077C440(Handle, VQA_Unknown_0077FD80, VQA_Unknown_0070B820);
            }
        } else if (state == 4) {
            VQA_Unknown_0070B690(unknown_508);
            if (VQA_Unknown_0071DBA0(11)) {
                VQA_Unknown_0077C440(Handle, VQA_Unknown_0070BF60, VQA_Unknown_0070C0A0);
            }
        }
    }
    int result = 0;
    while ((result >= 0 || result == -10 || result == -9 || result == -11 || result == -1) && !done) {
        VQA_Unknown_0071C590();
        if (sleeping == true) {
            if (unknown_flag_a40d28 == false) {
                Sleep(33);
                continue;
            }
            sleeping = false;
            unknown_548 = false;
            Debug_Printf("Movie is awake\n");
        }
        if (unknown_flag_a40d28 == true && !unknown_548) {
            result = VQA_Unknown_0077BF10(Handle, 1, 0);
            if (result >= 0) {
                if (PaletteChanged) {
                    Set_DirectDraw_Palette();
                    PaletteChanged = false;
                }
                if (Companion) {
                    Companion->Process_0069b420(VQA_Unknown_0077CBA0(Handle));
                }
                if (Callback540) {
                    Callback540();
                }
                CurrentFrame = result + 1;
            }
        } else if (!sleeping) {
            sleeping = true;
            if (Opened && !unknown_548) {
                VQA_Unknown_0077BF10(Handle, 2, 0);
            }
            unknown_548 = true;
            Debug_Printf("Movie is sleeping\n");
        }
        if (!unknown && WWKeyboard->Check() && WWKeyboard->Get() == 0x81B) {
            done = true;
        }
    }
    if (done) {
        return 1;
    }
    return 0;
}


// void VQAClass::Play_VQA_Frame(int frame) (0x0071CF50): seeks to the frame (-1: the last one) and plays it.

void VQAClass::Play_VQA_Frame(int frame)
{
    if (frame == -1) {
        frame = FrameCount - 1;
    }
    if (CurrentFrame != frame) {
        Seek_To_Frame(frame);
        CurrentFrame = frame;
    }
    Play_VQA(frame, 0);
    if ((unsigned)CurrentFrame >= (unsigned)FrameCount) {
        CurrentFrame = 0;
    }
}


// bool VQAClass::Advance_Frame(bool *unknown) (0x0071CFC0): VQAClass::Advance_Frame (debug strings);
// true when a frame was drawn; otherwise *unknown is set when the movie ended with an error.

bool VQAClass::Advance_Frame(bool *unknown)
{
    *unknown = false;
    if (!unknown_549) {
        VQA_Unknown_0077C8A0(Handle, FrameCount);
        if (unknown_508 != -1) {
            int unknown2;
            int unknown3;
            int state;
            VQA_Unknown_0077C920(Handle, &unknown3, &unknown2, &state);
            if (state == 1 || state == 4) {
                VQA_Unknown_0070B690(unknown_508);
                if (state == 1) {
                    VQA_Unknown_0077C440(Handle, VQA_Unknown_0077FD80, VQA_Unknown_0070B820);
                }
            }
        }
        Debug_Printf("VQAClass::Advance_Frame() - Ready\n");
        unknown_549 = true;
    }
    if (unknown_548 == true && !(unknown_flags_a3f600 & 2) && unknown_int_a40d48 == 0) {
        if (unknown_flag_a40d28 != true) {
            goto pause;
        }
        unknown_548 = false;
        Debug_Printf("VQAClass::Advance_Frame() - Unpause\n");
    }
    if (unknown_flag_a40d28 == true && !unknown_548) {
        int result = VQA_Unknown_0077BF10(Handle, 1, 0);
        if (result >= 0) {
            if (PaletteChanged) {
                Set_DirectDraw_Palette();
                PaletteChanged = false;
            }
            if (Companion) {
                Companion->Process_0069b420(VQA_Unknown_0077CBA0(Handle));
            }
            if (Callback540) {
                Callback540();
            }
            CurrentFrame = result + 1;
            return true;
        }
        switch (result) {
        case -11:
        case -10:
        case -1:
            *unknown = false;
            return false;
        default:
            *unknown = true;
            return false;
        }
    }
pause:
    if (!unknown_vqa_paused_af901c) {
        if (Opened && !unknown_548) {
            VQA_Unknown_0077BF10(Handle, 2, 0);
        }
        unknown_548 = true;
        unknown_vqa_paused_af901c = true;
        Debug_Printf("VQAClass::Advance_Frame() - Pause\n");
    }
    return false;
}


// void VQAClass::Pause_VQA() (0x0071D1A0): pause.

void VQAClass::Pause_VQA()
{
    if (Opened && !unknown_548) {
        VQA_Unknown_0077BF10(Handle, 2, 0);
    }
    unknown_548 = true;
}


// void VQAClass::Resume_VQA() (0x0071D1E0): resume.

void VQAClass::Resume_VQA()
{
    unknown_548 = false;
}


// void VQAClass::Close_And_Free_VQA() (0x0071D1F0): stop.

void VQAClass::Close_And_Free_VQA()
{
    if (Opened) {
        VQA_Unknown_0077BE70(Handle);
    }
}


// void VQAClass::Reset_VQA() (0x0071D210)

void VQAClass::Reset_VQA()
{
    VQA_Unknown_0077C7D0(Handle);
    if (Companion != 0) {
        Companion->Reset_0069b4e0();
    }
}


// int VQAClass::Get_Desired_Color_Mode() (0x0071D230)

int VQAClass::Get_Desired_Color_Mode()
{
    return unknown_504;
}


// bool VQAClass::Set_Draw_Buffer(int unknown1, int x, int y, int width, int height) (0x0071D240)
// -1 keeps the saved position and gives a zero size.

bool VQAClass::Set_Draw_Buffer(int unknown1, int x, int y, int width, int height)
{
    if (x == -1) {
        x = unknown_518;
    } else {
        unknown_518 = x;
    }
    if (y == -1) {
        y = unknown_51c;
    } else {
        unknown_51c = y;
    }
    if (width == -1) {
        width = 0;
    }
    if (height == -1) {
        height = 0;
    }
    return VQA_Unknown_0077C480(Handle, unknown1, x, y, width, height) == -1 ? true : false;
}


// void VQAClass::Set_Primary_Color_Mode(int value) (0x0071D2B0)

void VQAClass::Set_Primary_Color_Mode(int value)
{
    unknown_508 = value;
}


// void VQAClass::Handle_Codebook_Event(void *buffer, unsigned length) (0x0071D2C0)

void VQAClass::Handle_Codebook_Event(void *buffer, unsigned length)
{
    if (unknown_508 != -1) {
        VQA_Unknown_0070B7F0(buffer, length / 2);
    }
    unknown_544 = CurrentFrame;
}


// int VQAClass::Get_VQA_Width() (0x0071D300): width.

int VQAClass::Get_VQA_Width()
{
    return Width;
}


// int VQAClass::Get_VQA_Height() (0x0071D310): height.

int VQAClass::Get_VQA_Height()
{
    return Height;
}


// int VQAClass::Handle_Lock_Event() (0x0071D320): calls the callback at +0x538.

int VQAClass::Handle_Lock_Event()
{
    return Callback538();
}


// void VQAClass::Handle_Unlock_Event() (0x0071D330): calls the callback at +0x53C.

void VQAClass::Handle_Unlock_Event()
{
    Callback53C();
}


// int VQAClass::CCFileHandler(int request, int unknown2, int unknown3) (0x0071D340)
// Stream callback on the movie file (File); nonzero results are errors.

int VQAClass::CCFileHandler(int request, int unknown2, int unknown3)
{
    int error = 1;
    char byte;
    switch (request) {
    case 3:
        FileOpen = File.Open((char const *)unknown2, 1) != 0;
        error = !FileOpen;
        break;
    case 5:
        error = File.Read((void *)unknown2, unknown3) != unknown3;
        break;
    case 6:
        error = 1;
        break;
    case 7:
        error = File.Seek(unknown3, unknown2) == 0;
        break;
    case 8: {
        if (unknown3 > 0) {
            error = File.Seek(unknown3 - 1, unknown2) == 0;
            if (error) {
                break;
            }
            error = File.Read(&byte, 1) != 1;
        } else {
            error = File.Seek(unknown3, unknown2) == 0;
            if (error) {
                break;
            }
            error = File.Read(&byte, 1) != 1;
            if (error) {
                break;
            }
            error = File.Seek(-1, 1) == 0;
        }
        break;
    }
    case 9:
        *(long *)unknown2 = File.Size();
        error = 0;
        break;
    case 4:
        File.Close();
        FileOpen = false;
        error = 0;
        break;
    case 1:
    case 2:
        error = 0;
        break;
    }
    return error;
}


// int __cdecl VQA_Unknown_0071D490(VQAHandle *handle, int unknown2, int unknown3, int unknown4) (0x0071D490)
// Library stream callback: forwards to the owning player's 0x0071D340.

int __cdecl VQA_Unknown_0071D490(VQAHandle *handle, int unknown2, int unknown3, int unknown4)
{
    VQAClass *player = handle->Player;
    return player->CCFileHandler(unknown2, unknown3, unknown4);
}


// int VQAClass::MixFileHandler(int request, int unknown2, int unknown3) (0x0071D4B0)
// Stream callback on a movie inside a mixfile: opening seeks the mixfile
// to the movie; otherwise as 0x0071D340.
#include "shared/mix_file_class.hpp"

int VQAClass::MixFileHandler(int request, int unknown2, int unknown3)
{
    int error = 1;
    char byte;
    switch (request) {
    case 3: {
        long offset = 0;
        MixFileClass *mixfile = 0;
        if (MixFileClass::Offset(Filename, 0, &mixfile, &offset, 0)) {
            FileOpen = File.Open(mixfile->Filename, 1) != 0;
            return File.Seek(offset, 1) == 0;
        }
        return 1;
    }
    case 5:
        error = File.Read((void *)unknown2, unknown3) != unknown3;
        break;
    case 6:
        return 1;
    case 7:
        error = File.Seek(unknown3, unknown2) == 0;
        break;
    case 8: {
        if (unknown3 > 0) {
            error = File.Seek(unknown3 - 1, unknown2) == 0;
            if (error) {
                break;
            }
            error = File.Read(&byte, 1) != 1;
        } else {
            error = File.Seek(unknown3, unknown2) == 0;
            if (error) {
                break;
            }
            error = File.Read(&byte, 1) != 1;
            if (error) {
                break;
            }
            error = File.Seek(-1, 1) == 0;
        }
        break;
    }
    case 9:
        *(long *)unknown2 = File.Size();
        return 0;
    case 4:
        File.Close();
        FileOpen = false;
        error = 0;
        break;
    case 1:
    case 2:
        error = 0;
        break;
    }
    return error;
}


// int __cdecl VQA_Unknown_0071D640(VQAHandle *handle, int unknown2, int unknown3, int unknown4) (0x0071D640)
// Library callback: forwards to the owning player's 0x0071D4B0.

int __cdecl VQA_Unknown_0071D640(VQAHandle *handle, int unknown2, int unknown3, int unknown4)
{
    VQAClass *player = handle->Player;
    return player->MixFileHandler(unknown2, unknown3, unknown4);
}


// int __fastcall VQA_Unknown_0071D660(VQAHandle *handle, int unknown2, int unknown3, int unknown4) (0x0071D660)
// Library callback: forwards to the owning player's 0x0071CA00.

int __fastcall VQA_Unknown_0071D660(VQAHandle *handle, int unknown2, int unknown3, int unknown4)
{
    return handle->Player->CacheHandler(unknown2, unknown3, unknown4);
}


// int __cdecl VQA_Unknown_0071D680(VQAHandle *handle, int request, int unknown3, int unknown4) (0x0071D680)
// Library memory callback: 2 allocates unknown4 bytes, 3 frees unknown3,
// 4 and 5 return unknown3, 6 stores -1 through it.

extern "C" void *__cdecl malloc(unsigned size);
extern "C" void __cdecl free(void *block);

int __cdecl VQA_Unknown_0071D680(VQAHandle *handle, int request, int unknown3, int unknown4)
{
    switch (request) {
    case 2:
        return (int)malloc(unknown4);
    case 3:
        free((void *)unknown3);
        return 0;
    case 4:
    case 5:
        return unknown3;
    case 6:
        *(int *)unknown3 = -1;
        break;
    }
    return 0;
}


// int __cdecl VQA_Unknown_0071D6E0(VQAHandle *handle, int event, int unknown3, int unknown4) (0x0071D6E0)
// Library event callback: 2 a new palette (6-bit components scaled by 4),
// 3 frame timing (0x0071D2C0), 6 and 7 the player's callbacks.

int __cdecl VQA_Unknown_0071D6E0(VQAHandle *handle, int event, int unknown3, int unknown4)
{
    VQAClass *player = handle->Player;
    switch (event) {
    case 2: {
        unsigned char *palette = (unsigned char *)unknown3;
        for (int index = 0; index < 256; index++) {
            palette[index] <<= 2;
        }
        memcpy(player->Palette, palette, sizeof(player->Palette));
        player->PaletteChanged = true;
        return 0;
    }
    case 3:
        player->Handle_Codebook_Event((void *)unknown3, unknown4);
        return 0;
    case 4:
    case 5:
        break;
    case 6:
        return player->Callback538();
    case 7:
        player->Callback53C();
        break;
    }
    return 0;
}


// int VQAClass::Unknown_0071D790() (0x0071D790): returns 0 (no callers).

int VQAClass::Unknown_0071D790()
{
    return 0;
}


// void VQAClass::Unknown_0071D7A0() (0x0071D7A0): empty (no callers).

void VQAClass::Unknown_0071D7A0()
{
}


// void VQAClass::Unknown_0071D7B0() (0x0071D7B0): empty (no callers).

void VQAClass::Unknown_0071D7B0()
{
}


// DynamicVectorClass_MovieHandle_ptr_Clear_0071d7c0: instantiated in the VQA unit from shared/vector_class.hpp (the movie
// handle list 0x00AF8FD8); no source of its own.


// DynamicVectorClass_MovieHandle_ptr_ID_pointer_0071d7f0: instantiated in the VQA unit from shared/vector_class.hpp (the movie
// handle list 0x00AF8FD8); no source of its own.


// VectorClass_MovieHandle_ptr_Fetch_0071d810: instantiated in the VQA unit from shared/vector_class.hpp (the movie
// handle list 0x00AF8FD8); no source of its own.


// VectorClass_MovieHandle_ptr_operator_equal_0071d820: instantiated in the VQA unit from shared/vector_class.hpp (the movie
// handle list 0x00AF8FD8); no source of its own.


// VectorClass_MovieHandle_ptr_Resize_0071d860: instantiated in the VQA unit from shared/vector_class.hpp (the movie
// handle list 0x00AF8FD8); no source of its own.


// VectorClass_MovieHandle_ptr_Clear_0071d910: instantiated in the VQA unit from shared/vector_class.hpp (the movie
// handle list 0x00AF8FD8); no source of its own.


// VectorClass_MovieHandle_ptr_ID_object_0071d940: instantiated in the VQA unit from shared/vector_class.hpp (the movie
// handle list 0x00AF8FD8); no source of its own.


// VectorClass_MovieHandle_ptr_ID_pointer_0071d980: instantiated in the VQA unit from shared/vector_class.hpp (the movie
// handle list 0x00AF8FD8); no source of its own.


// DynamicVectorClass_MovieHandle_ptr_Resize_0071d9a0: instantiated in the VQA unit from shared/vector_class.hpp (the movie
// handle list 0x00AF8FD8); no source of its own.


// DynamicVectorClass_MovieHandle_ptr_ID_object_0071da50: instantiated in the VQA unit from shared/vector_class.hpp (the movie
// handle list 0x00AF8FD8); no source of its own.


// VectorClass_MovieHandle_ptr_scalar_deleting_destructor_0071da80: instantiated in the VQA unit from shared/vector_class.hpp (the movie
// handle list 0x00AF8FD8); no source of its own.


// DynamicVectorClass_MovieHandle_ptr_scalar_deleting_destructor_0071dad0: instantiated in the VQA unit from shared/vector_class.hpp (the movie
// handle list 0x00AF8FD8); no source of its own.
