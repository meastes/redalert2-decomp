// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// From mischa85/RedAlert2 392e904 src/movie.cpp (matching RA2 1.006 decomp; TibSun movie.cpp): function, global,
// parameter and local names and Stop_Ingame_Movie's call of Movie_Audio_Resume; modified
// (our module form and header state, Route A unit map manifests/original_target/modules/movie.json).

// Header-static dynamic initializers of the unit at 0x005A0060: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x005A0060-0x005A0241); the other
// drafts of this plan are emitted by these includes.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"


// Dynamic initializer (0x005A0080, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_005a0060.cpp; no source of its own.


// Dynamic initializer (0x005A00B0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_005a0060.cpp; no source of its own.


// Dynamic initializer (0x005A00D0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_005a0060.cpp; no source of its own.


// Dynamic initializer (0x005A00F0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_005a0060.cpp; no source of its own.


// Dynamic initializer (0x005A0110, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_005a0060.cpp; no source of its own.


// Dynamic initializer (0x005A0130, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_005a0060.cpp; no source of its own.


// Dynamic initializer (0x005A0150, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_005a0060.cpp; no source of its own.


// Dynamic initializer (0x005A0190, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_005a0060.cpp; no source of its own.


// Dynamic initializer (0x005A01C0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_005a0060.cpp; no source of its own.


// Dynamic initializer (0x005A01F0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_005a0060.cpp; no source of its own.


// Dynamic initializer (0x005A0220, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_005a0060.cpp; no source of its own.


// Dynamic initializer (0x005A0230, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_005a0060.cpp; no source of its own.




#include "shared/play_movie.hpp"
#include "shared/movie.hpp"
#include "shared/dsurface.hpp"
#include "shared/movie_globals.hpp"
#include "shared/audio.hpp"
#include "shared/audio_pause.hpp"
#include "shared/gscreen.hpp"
#include "shared/map_view.hpp"
#include "shared/mouse.hpp"
#include "shared/keyboard.hpp"
#include "shared/options.hpp"
#include "shared/hicolor.hpp"
#include "shared/crt_string.hpp"
#include "shared/debug_printf.hpp"
#include "shared/movie_list.hpp"
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_OBJECT
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#include "shared/movie_queue.hpp"
#include "shared/subtitle.hpp"
#include "shared/session_state.hpp"
#define VECTOR_IMPLEMENT_DYNAMIC_CLEAR
#include "shared/vox_speech.hpp"


// void __fastcall Rebuild_Interpolated_Palette(unsigned char *interpal) (0x005A0250)
// Mirrors the stored triangle of the 256 x 256 interpolation table across its diagonal.

void __fastcall Rebuild_Interpolated_Palette(unsigned char *interpal)
{
    for (int y = 0; y < 256 - 1; y++) {
        for (int x = y + 1; x < 256; x++) {
            *(interpal + (y * 256 + x)) = *(interpal + (x * 256 + y));
        }
    }
}


// void __fastcall Play_Movie(char const *name, int theme, bool clrscrn_after, bool stretch, bool clrscrn_before) (0x005A02B0)

void __fastcall Play_Movie(char const *name, int theme, bool clrscrn_after, bool stretch, bool clrscrn_before)
{
    Play_Movie(name, theme, clrscrn_after, stretch, clrscrn_before, true);
}


// void __fastcall Play_Movie(char const *name, int theme, bool clrscrn_after, bool stretch, bool clrscrn_before, bool fullscreen) (0x005A02D0)
// TibSun's Play_Movie with RA2's Bink branch: the game sound is paused around both players.

void __fastcall Play_Movie(char const *name, int theme, bool clrscrn_after, bool stretch, bool clrscrn_before,
                           bool fullscreen)
{
    char buffer[256];

    if (!Find_Movie_File(name, buffer, 1)) {
        return;
    }

    if (unknown_game_mode_a3d298 != 0) {
        return;
    }

    WWKeyboard->Clear();

    char *ext = strrchr(buffer, '.');
    if (ext != 0 && _stricmp(ext, ".bik") == 0) {
        Debug_Printf("Play_Movie() as Bink!\n");
        WWMouse->Hide_Mouse();
        if (clrscrn_before) {
            HiddenSurface->Fill(0);
            GScreenClass::Blit(true, HiddenSurface);
        }

        BinkClass bink(buffer);
        if (fullscreen == true) {
            bink.unknown_00 = true;
        }
        Movie_Audio_Suspend();
        Audio_Pause();
        Audio_DS_Set_Primary_Format(44100, 16, 2);
        bink.Play();
        Audio_DS_Apply_Primary_Format();
        Audio_Resume();
        Movie_Audio_Resume();

        if (clrscrn_after == true) {
            HiddenSurface->Fill(0);
            GScreenClass::Blit(true, HiddenSurface);
        }
        WWKeyboard->Clear();
        WWMouse->Show_Mouse();
        Map.Flag_To_Redraw(2);
        return;
    }

    WWKeyboard->Clear();
    VQAMovie *vqa = Movie_Create(buffer, AlternateSurface, Rect(0, 0, 0, 0), Rect(0, 0, 0, 0),
                                         int(Options.VoiceVolume * 255.0), 1);
    if (vqa != 0) {
        if (vqa->Player->Get_VQA_Width() < 320 && vqa->Player->Get_VQA_Height() < 200) {
            VQA_Movie_Close(vqa);
            delete vqa;
            return;
        }

        bool dostretch = (stretch == true && Options.StretchMovies == true);
        if (AllowStretchBlits_852fd6 == true && dostretch == true) {
            if (vqa->Source.Width < unknown_int_839648 && vqa->Source.Height < unknown_int_83964c) {
                float ratio = ((float)unknown_int_839648 / (float)vqa->Source.Width);
                vqa->Dest.Width = unknown_int_839648;
                vqa->Dest.Height = (int)(vqa->Source.Height * ratio);
                vqa->Dest.X = 0;
                vqa->Dest.Y = (unknown_int_83964c - vqa->Dest.Height) / 2;
                Debug_Printf("Stretching movie %dx%d -> %dx%d\n", vqa->Source.Width, vqa->Source.Height, vqa->Dest.Width, vqa->Dest.Height);
            }
        }

        WWMouse->Hide_Mouse();
        if (clrscrn_before) {
            HiddenSurface->Fill(0);
            GScreenClass::Blit(true, HiddenSurface);
        }
        Movie_Audio_Suspend();
        Audio_Pause();
        Audio_DS_Set_Primary_Format(44100, 16, 2);
        VQA_Movie_Play(vqa, true, theme, false);
        Audio_DS_Apply_Primary_Format();
        Audio_Resume();
        Movie_Audio_Resume();
        VQA_Movie_Close(vqa);
        delete vqa;

        if (clrscrn_after == true) {
            HiddenSurface->Fill(0);
            GScreenClass::Blit(true, HiddenSurface);
        }
        WWMouse->Show_Mouse();
        Map.Flag_To_Redraw(2);
        WWKeyboard->Clear();
    }
}


// void __fastcall Play_Movie_Simple(char const *name) (0x005A0710)
// Opens the movie on the hidden surface, clears the screen, plays it with the game sound paused and
// deletes it.

void __fastcall Play_Movie_Simple(char const *name)
{
    MovieHandle *movie = Create_Movie_Handle(name, HiddenSurface, 0);
    if (movie != 0) {
        HiddenSurface->Fill(0);
        GScreenClass::Blit(true, HiddenSurface);
        WWKeyboard->Clear();
        Movie_Audio_Suspend();
        movie->Play();
        Movie_Audio_Resume();
        WWKeyboard->Clear();
        delete movie;
    }
}


// void __fastcall Play_Movie(int vq, int theme, bool clrscrn_after, bool stretch, bool clrscrn_before) (0x005A07F0)
// Plays the named vq of a vq-list index without a theme and with TacticalActive cleared.

void __fastcall Play_Movie(int vq, int theme, bool clrscrn_after, bool stretch, bool clrscrn_before)
{
    if (vq >= 0 && vq < Movies.Count()) {
        bool active = TacticalActive;
        TacticalActive = false;
        Play_Movie(Movies[vq], -1, clrscrn_after, stretch, clrscrn_before, false);
        TacticalActive = active;
    }
}


// void __fastcall Play_Ingame_Movie(char const *name, bool loop) (0x005A0840)
// Outside a game, opens the movie on the sidebar surface, puts it at the radar's corner, sets its end callback
// (0x005A08F0) and loop, and adds it to the queued movies.

void __fastcall Play_Ingame_Movie(char const *name, bool loop)
{
    if (unknown_game_mode_a3d298 == 0) {
        MovieHandle *movie = Create_Movie_Handle(name, SidebarSurface, 0);
        if (movie != 0) {
            movie->Set_Position(SidebarMovieX - 3, SidebarMovieY - 1);
            movie->InitSubtitles((int)Ingame_Movie_Subtitle);
            movie->unknown_04 = loop;
            IngameVQ.Add(movie);
        }
    }
}


// void __fastcall Ingame_Movie_Subtitle(int movie) (0x005A08F0)
// The subtitle callback of the radar's queued movies: shows the subtitle's text in the session's message
// list for its display duration (the argument is the SubTitle the movie's companion reached).

void __fastcall Ingame_Movie_Subtitle(int movie)
{
    SubTitle *subtitle = (SubTitle *)movie;
    if (subtitle != 0) {
        unknown_session_a3d298.Messages.Add_Message(0, 0, subtitle->Get_Text(), 0, 0x4046, subtitle->Get_Display_Duration(), false);
    }
}


// void __fastcall Play_Ingame_Movie(int vq, bool loop) (0x005A0920)
// Queues the vq of a vq-list index for the radar.

void __fastcall Play_Ingame_Movie(int vq, bool loop)
{
    if (vq >= 0 && vq < Movies.Count()) {
        Play_Ingame_Movie(Movies[vq], loop);
    }
}


// void Stop_Ingame_Movie() (0x005A09E0)

void Stop_Ingame_Movie()
{
    for (int i = 0; i < IngameVQ.Count(); i++) {
        if (IngameVQ[i] != 0) {
            delete IngameVQ[i];
        }
    }
    IngameVQ.Clear();
    Movie_Audio_Resume();
}


// void __fastcall Pause_Ingame_Movie(bool pause) (0x005A0A90)
// Pauses or resumes the first queued movie.

void __fastcall Pause_Ingame_Movie(bool pause)
{
    if (Has_Ingame_Movies()) {
        IngameVQ[0]->Pause(pause);
    }
}


// bool Has_Ingame_Movies() (0x005A0AB0)

bool Has_Ingame_Movies()
{
    return IngameVQ.Count() > 0;
}


// void Movie_Audio_Resume() (0x005A0AC0)

void Movie_Audio_Resume()
{
    if (MovieAudioSuspended) {
        Audio_Streams_Release();
        Audio_Streams_Resume();
        Audio_Streams_Fade_In();
        Audio_Global_Levels[7]->Volume.Set_Target(0x4000);
        Audio_Global_Levels[5]->Volume.Set_Target(0x4000);
        unknown_tick_sound();
        MovieAudioSuspended = false;
    }
}


// void Movie_Audio_Suspend() (0x005A0B10)

void Movie_Audio_Suspend()
{
    if (!MovieAudioSuspended) {
        Speech_Hold();
        Audio_Streams_Hold();
        Audio_Streams_Pause();
        Audio_Global_Levels[7]->Volume.Set_Target(0x2000);
        Audio_Global_Levels[5]->Volume.Set_Target(0x2000);
        MovieAudioSuspended = true;
    }
}
