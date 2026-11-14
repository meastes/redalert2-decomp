// movie.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/movie.json carries the addresses and the rows that
// stay in their own files.


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


// void __fastcall Play_Movie(char const *name, int theme, bool clrscrn_after, bool stretch, bool clrscrn_before, bool bink_flag) (0x005A02D0)
// TibSun's Play_Movie with RA2's Bink branch: the game sound is paused around both players.

void __fastcall Play_Movie(char const *name, int theme, bool clrscrn_after, bool stretch, bool clrscrn_before,
                           bool bink_flag)
{
    char filename[256];

    if (!Find_Movie_File(name, filename, 1)) {
        return;
    }

    if (unknown_game_mode_a3d298 != 0) {
        return;
    }

    WWKeyboard->Clear();

    char *extension = strrchr(filename, '.');
    if (extension != 0 && _stricmp(extension, ".bik") == 0) {
        Debug_Printf("Play_Movie() as Bink!\n");
        WWMouse->Hide_Mouse();
        if (clrscrn_before) {
            unknown_surface_839954->Fill(0);
            GScreenClass::Blit(true, unknown_surface_839954);
        }

        BinkClass bink(filename);
        if (bink_flag == true) {
            bink.unknown_00 = true;
        }
        Unknown_Audio_Pause_005a0b10();
        Audio_Pause();
        Audio_DS_Set_Primary_Format(44100, 16, 2);
        bink.Play();
        Audio_DS_Apply_Primary_Format();
        Audio_Resume();
        Unknown_Audio_Resume_005a0ac0();

        if (clrscrn_after == true) {
            unknown_surface_839954->Fill(0);
            GScreenClass::Blit(true, unknown_surface_839954);
        }
        WWKeyboard->Clear();
        WWMouse->Show_Mouse();
        Map.Flag_To_Redraw(2);
        return;
    }

    WWKeyboard->Clear();
    VQAMovie *vqa = VQA_Unknown_005A1010(filename, unknown_surface_839958, Rect(0, 0, 0, 0), Rect(0, 0, 0, 0),
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
            unknown_surface_839954->Fill(0);
            GScreenClass::Blit(true, unknown_surface_839954);
        }
        Unknown_Audio_Pause_005a0b10();
        Audio_Pause();
        Audio_DS_Set_Primary_Format(44100, 16, 2);
        VQA_Movie_Play(vqa, true, theme, false);
        Audio_DS_Apply_Primary_Format();
        Audio_Resume();
        Unknown_Audio_Resume_005a0ac0();
        VQA_Movie_Close(vqa);
        delete vqa;

        if (clrscrn_after == true) {
            unknown_surface_839954->Fill(0);
            GScreenClass::Blit(true, unknown_surface_839954);
        }
        WWMouse->Show_Mouse();
        Map.Flag_To_Redraw(2);
        WWKeyboard->Clear();
    }
}


// void __fastcall Play_Movie_Handle(char const *name) (0x005A0710)
// Opens the movie on the hidden surface, clears the screen, plays it with the game sound paused and
// deletes it.

void __fastcall Play_Movie_Handle(char const *name)
{
    MovieHandle *movie = Create_Movie_Handle(name, unknown_surface_839954, 0);
    if (movie != 0) {
        unknown_surface_839954->Fill(0);
        GScreenClass::Blit(true, unknown_surface_839954);
        WWKeyboard->Clear();
        Unknown_Audio_Pause_005a0b10();
        movie->Play();
        Unknown_Audio_Resume_005a0ac0();
        WWKeyboard->Clear();
        delete movie;
    }
}


// void __fastcall Play_Movie(int movie, int theme, bool clrscrn_after, bool stretch, bool clrscrn_before) (0x005A07F0)
// Plays the named movie of a movie-list index without a theme and with unknown_flag_a40380 cleared.

void __fastcall Play_Movie(int movie, int theme, bool clrscrn_after, bool stretch, bool clrscrn_before)
{
    if (movie >= 0 && movie < unknown_names_a71050.Count()) {
        bool old = unknown_flag_a40380;
        unknown_flag_a40380 = false;
        Play_Movie(unknown_names_a71050[movie], -1, clrscrn_after, stretch, clrscrn_before, false);
        unknown_flag_a40380 = old;
    }
}


// void __fastcall Unknown_Queue_Movie_005a0840(char const *name, bool flag) (0x005A0840)
// Outside a game, opens the movie on the sidebar surface, puts it at the radar's corner, sets its end callback
// (0x005A08F0) and flag, and adds it to the queued movies.

void __fastcall Unknown_Queue_Movie_005a0840(char const *name, bool flag)
{
    if (unknown_game_mode_a3d298 == 0) {
        MovieHandle *movie = Create_Movie_Handle(name, unknown_surface_839948, 0);
        if (movie != 0) {
            movie->Set_Position(unknown_radar_x_8336cc - 3, unknown_radar_y_8336d0 - 1);
            movie->InitSubtitles((int)Unknown_Movie_Message_005a08f0);
            movie->unknown_04 = flag;
            queued_movies_00af8fd8.Add(movie);
        }
    }
}


// void __fastcall Unknown_Movie_Message_005a08f0(int movie) (0x005A08F0)
// The subtitle callback of the radar's queued movies: shows the subtitle's text in the session's message
// list for its display duration (the argument is the SubTitle the movie's companion reached).

void __fastcall Unknown_Movie_Message_005a08f0(int movie)
{
    SubTitle *subtitle = (SubTitle *)movie;
    if (subtitle != 0) {
        unknown_session_a3d298.Messages.Add_Message(0, 0, subtitle->Get_Text(), 0, 0x4046, subtitle->Get_Display_Duration(), false);
    }
}


// void __fastcall Play_Ingame_Movie(int movie, bool flag) (0x005A0920)
// Queues the movie of a movie-list index for the radar.

void __fastcall Play_Ingame_Movie(int movie, bool flag)
{
    if (movie >= 0 && movie < unknown_names_a71050.Count()) {
        Unknown_Queue_Movie_005a0840(unknown_names_a71050[movie], flag);
    }
}


// void Unknown_Clear_Movies_005a09e0() (0x005A09E0)

void Unknown_Clear_Movies_005a09e0()
{
    for (int i = 0; i < queued_movies_00af8fd8.Count(); i++) {
        if (queued_movies_00af8fd8[i] != 0) {
            delete queued_movies_00af8fd8[i];
        }
    }
    queued_movies_00af8fd8.Clear();
    if (unknown_audio_paused_a7101c) {
        Audio_Streams_Release();
        Audio_Streams_Resume();
        Audio_Streams_Fade_In();
        Audio_Global_Levels[7]->Volume.Set_Target(0x4000);
        Audio_Global_Levels[5]->Volume.Set_Target(0x4000);
        unknown_tick_sound();
        unknown_audio_paused_a7101c = false;
    }
}


// void __fastcall Pause_Ingame_Movie(bool pause) (0x005A0A90)
// Pauses or resumes the first queued movie.

void __fastcall Pause_Ingame_Movie(bool pause)
{
    if (Unknown_Movies_Queued_005a0ab0()) {
        queued_movies_00af8fd8[0]->Pause(pause);
    }
}


// bool Unknown_Movies_Queued_005a0ab0() (0x005A0AB0)

bool Unknown_Movies_Queued_005a0ab0()
{
    return queued_movies_00af8fd8.Count() > 0;
}


// void Unknown_Audio_Resume_005a0ac0() (0x005A0AC0)

void Unknown_Audio_Resume_005a0ac0()
{
    if (unknown_audio_paused_a7101c) {
        Audio_Streams_Release();
        Audio_Streams_Resume();
        Audio_Streams_Fade_In();
        Audio_Global_Levels[7]->Volume.Set_Target(0x4000);
        Audio_Global_Levels[5]->Volume.Set_Target(0x4000);
        unknown_tick_sound();
        unknown_audio_paused_a7101c = false;
    }
}


// void Unknown_Audio_Pause_005a0b10() (0x005A0B10)

void Unknown_Audio_Pause_005a0b10()
{
    if (!unknown_audio_paused_a7101c) {
        Speech_Hold();
        Audio_Streams_Hold();
        Audio_Streams_Pause();
        Audio_Global_Levels[7]->Volume.Set_Target(0x2000);
        Audio_Global_Levels[5]->Volume.Set_Target(0x2000);
        unknown_audio_paused_a7101c = true;
    }
}
