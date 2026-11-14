// movies.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/movies.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x005A0B50 and its
// list globals (char_const_ptr); the other drafts of this plan are emitted by this file.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"


// The unit's list of char const (0x00A71050; name is a hypothesis).
DynamicVectorClass<char const *> unknown_char_const_ptr_list_00a71050;


// Dynamic initializer (0x005A0B70, _$E5) emitted by HeaderStatics_empty_rect_init_005a0b50.cpp; no source of its own.


// Dynamic initializer (0x005A0BA0, _$E8) emitted by HeaderStatics_empty_rect_init_005a0b50.cpp; no source of its own.


// Dynamic initializer (0x005A0BC0, _$E11) emitted by HeaderStatics_empty_rect_init_005a0b50.cpp; no source of its own.


// Dynamic initializer (0x005A0BE0, _$E14) emitted by HeaderStatics_empty_rect_init_005a0b50.cpp; no source of its own.


// Dynamic initializer (0x005A0C00, _$E17) emitted by HeaderStatics_empty_rect_init_005a0b50.cpp; no source of its own.


// Dynamic initializer (0x005A0C20, _$E20) emitted by HeaderStatics_empty_rect_init_005a0b50.cpp; no source of its own.


// Dynamic initializer (0x005A0C40, _$E23) emitted by HeaderStatics_empty_rect_init_005a0b50.cpp; no source of its own.


// Dynamic initializer (0x005A0C80, _$E26) emitted by HeaderStatics_empty_rect_init_005a0b50.cpp; no source of its own.


// Dynamic initializer (0x005A0CB0, _$E29) emitted by HeaderStatics_empty_rect_init_005a0b50.cpp; no source of its own.


// Dynamic initializer (0x005A0CE0, _$E32) emitted by HeaderStatics_empty_rect_init_005a0b50.cpp; no source of its own.


// Dynamic initializer (0x005A0D10, _$E35) emitted by HeaderStatics_empty_rect_init_005a0b50.cpp; no source of its own.


// Dynamic initializer (0x005A0D20, _$E38) emitted by HeaderStatics_empty_rect_init_005a0b50.cpp; no source of its own.


// Dynamic initializer (0x005A0D40, _$E43) emitted by HeaderStatics_empty_rect_init_005a0b50.cpp; no source of its own.


// atexit destructor (0x005A0D80, _$E41) emitted by HeaderStatics_empty_rect_init_005a0b50.cpp; no source of its own.


#include "shared/movie.hpp"
#include "shared/win32_api.hpp"
#include "shared/dsurface.hpp"
#include "shared/hicolor.hpp"
#include "shared/sound_available.hpp"
#include "shared/rect_intersect.hpp"
#include "shared/movie_globals.hpp"
#include "shared/options.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/crt_string.hpp"
#include "shared/debug_printf.hpp"


// int __fastcall Movie_Lock_Surface() (0x005A0DC0)
// The VQA player's lock callback (TibSun Movie_Lock_Surface): locks the current movie's frame surface and
// points the player's draw buffer at it (pitch in pixels, height, the source corner); the buffer address.

int __fastcall Movie_Lock_Surface()
{
    Surface *surface = CurrentVQAMovie->FrameSurface;
    void *buffer = surface->Lock();
    VQAClass *player = CurrentVQAMovie->Player;
    player->Set_Draw_Buffer(0, surface->Stride() / surface->Bytes_Per_Pixel(), surface->Get_Height(),
                             CurrentVQAMovie->Source.X, CurrentVQAMovie->Source.Y);
    return (int)buffer;
}


// void Movie_Unlock_Surface() (0x005A0E20)
// The VQA player's unlock callback (TibSun Movie_Unlock_Surface): unlocks the current movie's frame surface.

void Movie_Unlock_Surface()
{
    CurrentVQAMovie->FrameSurface->Unlock();
}


// void Movie_Blit_To_Screen() (0x005A0E30)
// The VQA player's frame callback for windowed movies (TibSun Movie_Blit_To_Screen): unless option 15 is
// set, copies the frame's source area to the movie's destination in the window's client area.

void Movie_Blit_To_Screen()
{
    RECT rect;
    if (!VQA_Unknown_0071DBA0(15)) {
        GetClientRect(MainWindow, &rect);
        ClientToScreen(MainWindow, (POINT *)&rect);
        Rect area;
        area.Set(rect.left + CurrentVQAMovie->Dest.X, rect.top + CurrentVQAMovie->Dest.Y,
                 CurrentVQAMovie->Dest.Width, CurrentVQAMovie->Dest.Height);
        unknown_surface_839950->Copy_From(area, *CurrentVQAMovie->FrameSurface, CurrentVQAMovie->Source, false, true);
    }
}


// void Movie_Blit_To_Screen_Hidden() (0x005A0EC0)
// The VQA player's frame callback for full-screen movies: unless option 15 is set, copies the frame to the
// hidden surface, draws the companion's picture centred at its bottom, then copies the hidden surface's
// source area to the movie's destination in the window's client area.

void Movie_Blit_To_Screen_Hidden()
{
    RECT rect;
    if (!VQA_Unknown_0071DBA0(15)) {
        unknown_surface_839954->Copy_From(CurrentVQAMovie->Source, *CurrentVQAMovie->FrameSurface,
                                          CurrentVQAMovie->Source, false, true);
        if (CurrentVQAMovie->Companion != 0 && CurrentVQAMovie->Companion->unknown_00 != 0) {
            Surface *picture = CurrentVQAMovie->Companion->unknown_10;
            Rect area = picture->Get_Rect();
            Rect dest;
            dest.X = (unknown_surface_839954->Get_Width() - area.Width) / 2;
            dest.Y = unknown_surface_839954->Get_Height() - area.Height;
            dest.Width = area.Width;
            dest.Height = area.Height;
            unknown_surface_839954->Copy_From(dest, *picture, area, true, true);
        }
        GetClientRect(MainWindow, &rect);
        ClientToScreen(MainWindow, (POINT *)&rect);
        Rect area;
        area.Set(rect.left + CurrentVQAMovie->Dest.X, rect.top + CurrentVQAMovie->Dest.Y,
                 CurrentVQAMovie->Dest.Width, CurrentVQAMovie->Dest.Height);
        unknown_surface_839950->Copy_From(area, *unknown_surface_839954, CurrentVQAMovie->Source, false, true);
    }
}


// VQAMovie *__fastcall Movie_Create(char const *name, Surface *surface, Rect source, Rect dest, int volume,
// bool fullscreen) (0x005A1010)
// TibSun Movie_Create: a new VQA movie for name drawing onto surface, its player locking the movie's frame
// surface (fullscreen: Movie_Blit_To_Screen_Hidden copies each frame to the screen); an empty source is the
// whole movie, an empty dest the movie centred on surface, both clipped. The subtitle companion is created
// from the name. Null when the file is missing or the movie does not open.

int MovieInt1;      // 0x00A710B4 (TibSun MovieInt1: 1 adds player flag 2)

int __fastcall Movie_Lock_Surface();
// The VQAClass constructor takes the lock callback as int (*)(); this one is __fastcall (cast below).
void Movie_Unlock_Surface();
void Movie_Blit_To_Screen_Hidden();

// TibSun rect.h's two-argument Intersect, a wrapper over the four-argument form (shared/rect.hpp declares that one
// with default x/y, so the wrapper takes its own name here). The wrapper puts the inline Intersect one level deeper:
// the first call stays out of line (0x0041FDF0), the second is expanded.
inline Rect Intersect_Rects(Rect const &rect1, Rect const &rect2)
{
    return Intersect(rect1, rect2, (int *)0, (int *)0);
}

VQAMovie *__fastcall VQA_Unknown_005A1010(char const *name, Surface *surface, Rect source, Rect dest, int volume,
                                          bool fullscreen)
{
    void (*callback)() = 0;
    VQAMovie *movie = new VQAMovie;
    if (movie != 0) {
        if (!CCFileClass(name).Is_Available()) {
            return 0;
        }
        int flags = 0;

        if (!unknown_check_406f30()) {
            VQA_Unknown_0071DB60(3);
        }

        if (VQA_Unknown_0071DBA0(0)) {
            flags |= 1;
        }

        if (MovieInt1 == 1) {
            flags |= 2;
        }

        if (fullscreen == true) {
            callback = Movie_Blit_To_Screen_Hidden;
        }

        movie->Player = new VQAClass(name, flags, (int (*)())Movie_Lock_Surface, Movie_Unlock_Surface, callback,
                                     -1, -1);
        if (movie->Player == 0) {
            delete movie;
            return 0;
        }

        movie->Player->Set_VQA_Volume(volume);

        if (!movie->Player->Open_And_Load_Buffers()) {
            delete movie;
            return 0;
        }

        int width = movie->Player->Get_VQA_Width();
        int height = movie->Player->Get_VQA_Height();
        int cmode = movie->Player->Get_Desired_Color_Mode();

        if (cmode == 4) {
            cmode = 1;
        }

        if (source == Rect(0, 0, 0, 0)) {
            source = Rect(0, 0, width, height);
        }

        if (dest == Rect(0, 0, 0, 0)) {
            dest = Rect((surface->Get_Width() - width) / 2, (surface->Get_Height() - height) / 2, width, height);
        }

        Rect irect1(0, 0, width, height);
        Rect irect2(0, 0, surface->Get_Width(), surface->Get_Height());

        movie->Source = Intersect_Rects(source, irect1);
        movie->Dest = Intersect_Rects(dest, irect2);

        if (cmode == 1 || cmode == 4) {
            movie->Player->Set_Primary_Color_Mode(Get_Unknown_7d6290());
        }

        movie->FrameSurface = surface;
        movie->Player->Set_Draw_Buffer(0, surface->Stride() / surface->Bytes_Per_Pixel(), surface->Get_Height(),
                                        0, 0);
        movie->Companion = unknown_create_companion_69b190(name);
        if (movie->Companion != 0) {
            movie->Player->Companion = movie->Companion;
        }
        movie->Playing = true;
        return movie;
    }
    return 0;
}


// void __fastcall VQA_Movie_Close(VQAMovie *movie) (0x005A1470)
// Stops and frees the player and the companion object; clears Playing.

void __fastcall VQA_Movie_Close(VQAMovie *movie)
{
    if (movie) {
        if (movie->Player) {
            movie->Player->Close_And_Free_VQA();
            delete movie->Player;
            movie->Player = 0;
        }
        if (movie->Companion) {
            delete movie->Companion;
            movie->Companion = 0;
        }
        movie->Playing = false;
    }
}


// void __fastcall VQA_Movie_Play(VQAMovie *movie, bool hide, int theme, bool unknown) (0x005A14D0)
// Plays the movie to the end (or Repeats times) unless another one is
// playing; optionally brackets it with WWMouse Hide_Mouse/Show_Mouse and
// starts theme first.

VQAMovie *CurrentVQAMovie;      // 0x00A710AC: the VQA movie playing now

void __fastcall VQA_Movie_Play(VQAMovie *movie, bool hide, int theme, bool unknown)
{
    if (CurrentVQAMovie) {
        return;
    }

    movie->Playing = true;
    CurrentVQAMovie = movie;
    int start = 0;
    int unknown2 = -1;
    int repeats = 0;
    if (VQA_Unknown_0071DBA0(2)) {
        start = movie->unknown_10;
        if (movie->unknown_14 >= 0) {
            unknown2 = movie->unknown_14;
        }
        repeats = movie->Repeats;
        if (movie->Mode == 1) {
            if (movie->unknown_18 == 0) {
                movie->Player->Set_Loop(movie->unknown_20, repeats);
            } else {
                movie->Player->Set_Loop(start, unknown2, repeats);
            }
        }
    }

    if (hide) {
        WWMouse->Hide_Mouse();
    }
    if (theme != -1) {
        Theme.Queue_Song(theme);
    }

    do {
        if (movie->Mode == 2 || start) {
            movie->Player->Seek_To_Frame(start);
        }
        int result = movie->Player->Play_VQA(unknown2, unknown);
        if (movie->Mode == 1) {
            repeats = 0;
        }
        movie->Player->Reset_VQA();
        if (result == 1) {
            break;
        }
        if (repeats > 0) {
            repeats--;
        }
    } while (repeats != 0);

    if (hide) {
        WWMouse->Show_Mouse();
    }
    CurrentVQAMovie = 0;
}


// bool __fastcall VQA_Movie_Unknown_005A15F0(VQAMovie *movie, bool *unknown) (0x005A15F0)
// Runs one player step (0x0071CFC0) as the current movie unless another
// movie is playing.

bool __fastcall VQA_Movie_Unknown_005A15F0(VQAMovie *movie, bool *unknown)
{
    if (CurrentVQAMovie) {
        return false;
    }
    CurrentVQAMovie = movie;
    bool result = movie->Player->Advance_Frame(unknown);
    CurrentVQAMovie = 0;
    return result;
}


// bool __fastcall VQA_Movie_Is_Playing() (0x005A1620)
// Whether a VQA movie is playing.

bool __fastcall VQA_Movie_Is_Playing()
{
    return CurrentVQAMovie != 0;
}


// void __fastcall VQA_Movie_Pause(VQAMovie *movie) (0x005A1630)
// Pauses the movie's player, if any.

void __fastcall VQA_Movie_Pause(VQAMovie *movie)
{
    if (movie && movie->Player) {
        movie->Player->Pause_VQA();
    }
}


// void __fastcall VQA_Movie_Resume(VQAMovie *movie) (0x005A1640)
// Resumes the movie's player, if any.

void __fastcall VQA_Movie_Resume(VQAMovie *movie)
{
    if (movie && movie->Player) {
        movie->Player->Resume_VQA();
    }
}


// void __fastcall VQA_Movie_Blit() (0x005A1650)
// Copies the current movie frame to the screen surface at its window position.

void __fastcall VQA_Movie_Blit()
{
    if (CurrentVQAMovie) {
        unknown_surface_839958->Fill(0);
        Unknown_004E0C90(false, unknown_surface_839958, 0);
        if (!VQA_Unknown_0071DBA0(15)) {
            RECT client;
            GetClientRect(MainWindow, &client);
            ClientToScreen(MainWindow, (POINT *)&client);
            Rect dest(CurrentVQAMovie->Dest.X + client.left, CurrentVQAMovie->Dest.Y + client.top,
                      CurrentVQAMovie->Dest.Width, CurrentVQAMovie->Dest.Height);
            unknown_surface_839950->Copy_From(dest, *CurrentVQAMovie->FrameSurface, CurrentVQAMovie->Source, false, true);
        }
    }
}


// VQAMovie::VQAMovie() (0x005A1710)
// TibSun VQHandle::VQHandle: clears the wrapper; Repeats starts at 1 and unknown_04 at -1.

VQAMovie::VQAMovie() :
    Player(0),
    unknown_04(-1),
    FrameSurface(0),
    Mode(0),
    unknown_10(0),
    unknown_14(0),
    unknown_18(0),
    Repeats(1),
    unknown_20(0),
    Source(0, 0, 0, 0),
    Dest(0, 0, 0, 0),
    unknown_44(false),
    Playing(false),
    Companion(0)
{
}


// VQAMovie::~VQAMovie() (0x005A1760)
// Stops and frees the player and the companion object.

VQAMovie::~VQAMovie()
{
    if (Player) {
        Player->Close_And_Free_VQA();
        delete Player;
    }
    if (Companion) {
        delete Companion;
        Companion = 0;
    }
}


// VQMovieHandle::VQMovieHandle(VQAMovie *movie) (0x005A17B0)
// Wraps an opened VQA movie and takes its frame size from the player.

VQMovieHandle::VQMovieHandle(VQAMovie *movie) :
    Finished(false)
{
    Movie = movie;
    Width = Movie->Player->Get_VQA_Width();
    Height = Movie->Player->Get_VQA_Height();
}


// void VQMovieHandle::Pause(bool pause) (0x005A17F0)
// Pauses or resumes the movie.

void VQMovieHandle::Pause(bool pause)
{
    if (pause) {
        VQA_Movie_Pause(Movie);
    } else {
        VQA_Movie_Resume(Movie);
    }
}


// void VQMovieHandle::Play() (0x005A1830)
// Plays the whole movie as the current handle.

MovieHandle *CurrentMovieHandle;   // 0x00A710B0: the handle playing now

void VQMovieHandle::Play()
{
    CurrentMovieHandle = this;
    VQA_Movie_Play(Movie, true, -1, 0);
    CurrentMovieHandle = 0;
}


// bool VQMovieHandle::Is_Finished() (0x005A1850)

bool VQMovieHandle::Is_Finished()
{
    return Finished;
}


// bool VQMovieHandle::AdvanceFrame() (0x005A1860)
// Steps the movie while it is playing; the player reports Finished.

bool VQMovieHandle::AdvanceFrame()
{
    if (!Movie->Player->unknown_548 && Movie->Playing) {
        return VQA_Movie_Unknown_005A15F0(Movie, &Finished);
    }
    return false;
}


// void VQMovieHandle::Waiting() (0x005A18A0)
// Nothing to do for VQA movies.

void VQMovieHandle::Waiting()
{
}


// void VQMovieHandle::Set_Position(int x, int y) (0x005A18B0)
// Moves the frame; the destination follows the source rectangle.

void VQMovieHandle::Set_Position(int x, int y)
{
    Movie->Source.X = x;
    Movie->Source.Y = y;
    Movie->Dest = Movie->Source;
}


// void VQMovieHandle::InitSubtitles(int unknown) (0x005A18F0)
// Passes the value to the movie's companion object.

void VQMovieHandle::InitSubtitles(int unknown)
{
    Movie->Companion->SetCallback_0069b4f0(unknown);
}


// void VQMovieHandle::SeekToFrame(int frame) (0x005A1910)
// Seeks the VQA player to a frame (0x0071D210, then 0x0071CC60).

void VQMovieHandle::SeekToFrame(int frame)
{
    Movie->Player->Reset_VQA();
    Movie->Player->Seek_To_Frame(frame);
}


// int VQMovieHandle::Timing() (0x005A1930)

int VQMovieHandle::Timing()
{
    return 0x21;
}


// void VQMovieHandle::Blit() (0x005A1940)

void VQMovieHandle::Blit()
{
    VQA_Movie_Blit();
}


// VQMovieHandle::~VQMovieHandle() (0x005A1A00)
// Closes the movie (the wrapper itself is not freed).

VQMovieHandle::~VQMovieHandle()
{
    VQA_Movie_Close(Movie);
}


// BinkMovieHandle::BinkMovieHandle(BinkClass *bink) (0x005A1A80)

BinkMovieHandle::BinkMovieHandle(BinkClass *bink)
{
    Bink = bink;
    Width = Bink->Handle->Width;
    Height = Bink->Handle->Height;
}


// void BinkMovieHandle::Pause(bool pause) (0x005A1AB0)
// Forwards to the Bink wrapper.

void BinkMovieHandle::Pause(bool pause)
{
    Bink->Pause(pause);
}


// void BinkMovieHandle::Play() (0x005A1AC0)
// Plays the Bink movie as the current handle.

void BinkMovieHandle::Play()
{
    CurrentMovieHandle = this;
    Bink->Play();
    CurrentMovieHandle = 0;
}


// bool BinkMovieHandle::Is_Finished() (0x005A1AE0)
// Forwards to the Bink wrapper.

bool BinkMovieHandle::Is_Finished()
{
    return Bink->Is_Finished();
}


// bool BinkMovieHandle::AdvanceFrame() (0x005A1AF0)
// Forwards to the Bink wrapper.

bool BinkMovieHandle::AdvanceFrame()
{
    return Bink->Render_Frame();
}


// void BinkMovieHandle::Waiting() (0x005A1B00)
// Forwards to the Bink wrapper.

void BinkMovieHandle::Waiting()
{
    Bink->Check_Frame_Ready();
}


// void BinkMovieHandle::Set_Position(int x, int y) (0x005A1B10)
// Forwards to the Bink wrapper.

void BinkMovieHandle::Set_Position(int x, int y)
{
    Bink->Set_Position(x, y);
}


// void BinkMovieHandle::InitSubtitles(int unknown) (0x005A1B30)
// Forwards to the Bink wrapper.

void BinkMovieHandle::InitSubtitles(int unknown)
{
    Bink->Set_Subtitle_Callback(unknown);
}


// void BinkMovieHandle::SeekToFrame(int frame) (0x005A1B40)
// Forwards the seek to the Bink wrapper.

void BinkMovieHandle::SeekToFrame(int frame)
{
    Bink->Goto_Frame(frame);
}


// int BinkMovieHandle::Timing() (0x005A1B50)
// Forwards to the Bink wrapper.

int BinkMovieHandle::Timing()
{
    return Bink->Frame_Time();
}


// void BinkMovieHandle::Blit() (0x005A1B60)
// Forwards to the Bink wrapper.

void BinkMovieHandle::Blit()
{
    Bink->Redraw();
}


// BinkMovieHandle::~BinkMovieHandle() (0x005A1B70)
// Deletes the Bink wrapper.

BinkMovieHandle::~BinkMovieHandle()
{
    delete Bink;
}


// MovieHandle::~MovieHandle() (0x005A1BA0)

MovieHandle::~MovieHandle()
{
}


// bool __fastcall Find_Movie_File(char const *name, char *found, int unused) (0x005A1BB0)
// Cuts the name at its extension, unlocks the movie in the options, then looks for "<name>.BIK" and, failing
// that, "<name>.VQA"; copies the name found into found when given.

bool __fastcall Find_Movie_File(char const *name, char *found, int unused)
{
    char filename[200];

    strcpy(filename, name);
    char *extension = filename;
    while (*extension != '\0' && *extension != '.') {
        extension++;
    }
    *extension = '\0';
    Options.Unlock_Movie(filename);

    strcpy(extension, ".BIK");
    if (CCFileClass(filename).Is_Available()) {
        if (found != 0) {
            strcpy(found, filename);
        }
        return true;
    }
    strcpy(extension, ".VQA");
    if (CCFileClass(filename).Is_Available()) {
        if (found != 0) {
            strcpy(found, filename);
        }
        return true;
    }
    return false;
}


// MovieHandle *__fastcall Create_Movie_Handle(char const *name, Surface *surface, Rect const *area) (0x005A1D30)
// Opens the movie file found for name as a Bink or VQA movie playing onto
// surface (in area, or the whole surface).

extern "C" char *__cdecl strrchr(char const *string, int character);
extern "C" int __cdecl _stricmp(char const *string1, char const *string2);
VQAMovie *__fastcall VQA_Unknown_005A1010(char const *filename, Surface *surface, Rect unknown1, Rect area,
                                          int unknown2, bool unknown3);   // 0x005A1010

MovieHandle *__fastcall Create_Movie_Handle(char const *name, Surface *surface, Rect const *area)
{
    char filename[200];

    if (!surface || !Find_Movie_File(name, filename, 1)) {
        return 0;
    }

    Rect rect;
    if (area) {
        rect = *area;
    } else {
        rect = surface->Get_Rect();
    }

    char *extension = strrchr(filename, '.');
    if (extension && !_stricmp(extension, ".bik")) {
        Debug_Printf("Play_Movie() as Bink!\n");
        BinkClass *bink = new BinkClass(filename, surface);
        return new BinkMovieHandle(bink);
    }

    VQAMovie *movie = VQA_Unknown_005A1010(filename, surface, Rect(0, 0, 0, 0), rect, 0xFF, 0);
    if (!movie) {
        return 0;
    }
    return new VQMovieHandle(movie);
}


// int __fastcall Is_Movie_Active() (0x005A1ED0)
// Whether a VQA movie or a movie handle is playing.

int __fastcall Is_Movie_Active()
{
    if (CurrentVQAMovie || CurrentMovieHandle) {
        return 1;
    }
    return 0;
}


// void __fastcall Movie_Handle_Blit() (0x005A1EF0)
// Draws the frame of the playing movie handle, if any.

void __fastcall Movie_Handle_Blit()
{
    if (CurrentMovieHandle) {
        CurrentMovieHandle->Blit();
    }
}


// VQMovieHandle::`scalar deleting destructor'(unsigned int) (0x005A1F00)
// Compiler-generated from the virtual destructor; emitted with the vtable.


// BinkMovieHandle::`scalar deleting destructor'(unsigned int) (0x005A1F80)
// Compiler-generated from the virtual destructor; emitted with the vtable.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<char const *>::Clear (0x005A1FC0; DynamicVectorClass<char const *> vtable 0x007a65e4 slot 3).
// DynamicVectorClass<char const *>::ID_pointer (0x005A1FF0; DynamicVectorClass<char const *> vtable 0x007a65e4 slot 5).
// VectorClass<char const *>::Fetch (0x005A2010; VectorClass<char const *> vtable 0x007a6604 slot 6).
// VectorClass<char const *>::operator equal (0x005A2020; VectorClass<char const *> vtable 0x007a6604 slot 1).
// VectorClass<char const *>::Resize (0x005A2060; VectorClass<char const *> vtable 0x007a6604 slot 2).
// VectorClass<char const *>::Clear (0x005A2110; VectorClass<char const *> vtable 0x007a6604 slot 3).
// VectorClass<char const *>::ID_object (0x005A2140; VectorClass<char const *> vtable 0x007a6604 slot 4).
// VectorClass<char const *>::ID_pointer (0x005A2180; VectorClass<char const *> vtable 0x007a6604 slot 5).
// DynamicVectorClass<char const *>::Resize (0x005A21A0; DynamicVectorClass<char const *> vtable 0x007a65e4 slot 2).
// DynamicVectorClass<char const *>::ID_object (0x005A2250; DynamicVectorClass<char const *> vtable 0x007a65e4 slot 4).


extern "C" unsigned char vtable_5a2280[]; extern "C" void __cdecl release_buffer_5a2280(void*); extern "C" void __cdecl release_object_5a2280(void*);
class Leaf_5a2280 { public: void* get(unsigned int flags); };
void* Leaf_5a2280::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_5a2280; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_5a2280((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_5a2280(this); return this; }


// 0x005a22d0: function not yet identified
//
// Owner: DynamicVectorClass<char *>, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 73 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall FUN_005a22d0(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005a22d0 {

extern void  FUN_00781d3b(unsigned int allocation_address);
extern "C" void* PTR_FUN_007a6604;

struct Owner_005a22d0 { unsigned int * FUN_005a22d0(unsigned char deletion_flags); };

unsigned int * Owner_005a22d0::FUN_005a22d0(unsigned char deletion_flags)
{
    unsigned int * object_words = (unsigned int *)this;
    *object_words = (unsigned int)(&PTR_FUN_007a6604);
    if ((object_words[1] != 0) && (*(char *)((int)object_words + 0xd) != '\0')) {
        FUN_00781d3b((unsigned int)(object_words[1]));
        object_words[1] = 0;
    }
    *(unsigned char *)((int)object_words + 0xd) = 0;
    object_words[2] = 0;
    if ((deletion_flags & 1) != 0) {
        FUN_00781d3b((unsigned int)(object_words));
    }
    return object_words;
}

}  // namespace seed_005a22d0
