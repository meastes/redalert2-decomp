// theme.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/theme.json carries the addresses and the rows that
// stay in their own files.


#define THEME_CONTROL_RECORD
#include "shared/theme.hpp"
#include "shared/ccini_class.hpp"
#include "shared/vector_class.hpp"
#include "shared/audio.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/sound_available.hpp"
#include "shared/debug_printf.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/random2.hpp"
#include "shared/cd.hpp"
#include "shared/com_persist.hpp"
#include "shared/main_loop.hpp"


// ThemeControl::ThemeControl() (0x006E7740)
// An empty normal theme for any side (Init_Themes expands it inline).

ThemeControl::ThemeControl() :
    Scenario(0),
    Duration(0),
    unknown_288(true),
    unknown_289(false),
    Available(false),
    Owner(-1)
{
    Name[0] = '\0';
    Sound[0] = '\0';
    FullName[0] = 0;
}


// bool ThemeControl::Fill_In(CCINIClass const &ini) (0x006E7780)
// Reads the theme's section: the sound file (leading '$' and '#' dropped),
// scenario, normal and repeat flags, side and, for a normal theme, its
// display name.

extern "C" char *__cdecl strcpy(char *dest, char const *source);
#pragma intrinsic(strcpy)

bool ThemeControl::Fill_In(CCINIClass const &ini)
{
    if (ini.Is_Present(Name)) {
        ini.Get_String(Name, "Sound", "", Sound, sizeof(Sound));
        char *sound = Sound;
        while (*sound == '$' || *sound == '#') {
            sound++;
        }
        strcpy(Sound, sound);
        Scenario = ini.Get_Int(Name, "Scenario", Scenario);
        unknown_288 = ini.Get_Bool(Name, "Normal", unknown_288);
        unknown_289 = ini.Get_Bool(Name, "Repeat", unknown_289);
        Owner = ini.Get_Side(Name, "Side", Owner);
        if (unknown_288) {
            ini.Get_Wide_String(Name, "Name", FullName, sizeof(FullName) / sizeof(FullName[0]));
        }
        return true;
    }
    return false;
}


// void ThemeClass::Unknown_006E7890(CCINIClass &ini) (0x006E7890)
// Tiberian Sun's Init_Themes: reads every theme listed under [Themes]
// (new records for unknown names), then opens the music stream.
#ifndef VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#endif
#ifndef VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#endif
#ifndef VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#endif

extern "C" char *__cdecl strcpy(char *dest, char const *source);
#pragma intrinsic(strcpy)

// Two of the eight global level sets (0x0083144C-0x00831468; shared/audio.hpp).
extern AudioLevelSet *unknown_audio_levels_831454;
extern AudioLevelSet *unknown_audio_levels_831464;

void ThemeClass::Unknown_006E7890(CCINIClass &ini)
{
    ThemeControl *ctrl;
    int count = ini.Entry_Count("Themes");
    for (int index = 0; index < count; index++) {
        char name[32];
        if (ini.Get_String("Themes", ini.Get_Entry("Themes", index), "", name, sizeof(name))) {
            ThemeType theme = From_Name(name);
            if (theme == THEME_NONE) {
                ctrl = new ThemeControl;
                strcpy(ctrl->Name, name);
                Themes.Add(ctrl);
            } else {
                ctrl = Themes[theme];
            }
            ctrl->Fill_In(ini);
        }
    }
    if (Audio_Get_Device() != 0) {
        Sample = Audio_Stream_Create(Audio_Get_Device(), 20000);
        if (Sample != 0) {
            Audio_Stream_Set_Name((AudioStreamPlayer *)Sample, "Music Stream");
            Audio_Stream_Set_Levels_94((AudioStreamPlayer *)Sample, unknown_audio_levels_831454);
            Audio_Stream_Set_Levels_9c((AudioStreamPlayer *)Sample, unknown_audio_levels_831464);
        }
    }
}


// void ThemeClass::Unknown_006E7A70() (0x006E7A70)
// Frees every theme record, then fades out, waits for and destroys the stream.
#ifndef VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#endif
#ifndef VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#endif
#ifndef VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#endif
#ifndef VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#endif
#ifndef VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#endif

void __fastcall unknown_stop_sample_407e60(void *sample);
void __fastcall unknown_sample_407fe0(void *sample);
void __fastcall unknown_sample_408010(void *sample);
void __fastcall unknown_sample_4079c0(void *sample);

void ThemeClass::Unknown_006E7A70()
{
    while (Themes.Count() > 0) {
        delete Themes[0];
        Themes.Delete(0);
    }
    if (Sample != 0) {
        unknown_sample_407fe0(Sample);
        unknown_sample_408010(Sample);
        unknown_stop_sample_407e60(Sample);
        unknown_sample_4079c0(Sample);
        Sample = 0;
    }
}


// void ThemeClass::Scan() (0x006E7AF0)
// Marks each theme whose .WAV file exists as available and measures its
// length in seconds.
#ifndef VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#endif
#ifndef VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#endif

extern char unknown_sound_enabled_a40c18;
extern char unknown_flag_a40d0c;
extern "C" void __cdecl _makepath(char *path, char const *drive, char const *dir, char const *fname, char const *ext);

void ThemeClass::Scan()
{
    if (unknown_sound_enabled_a40c18 && unknown_check_406f30() && !unknown_flag_a40d0c) {
        for (int theme = 0; theme < Themes.Count(); theme++) {
            char name[512];
            _makepath(name, 0, 0, Themes[theme]->Sound, ".WAV");
            CCFileClass file(name);
            Themes[theme]->Available = file.Is_Available();
            if (Themes[theme]->Available) {
                AudioFormat format;
                Audio_Format_Init(&format);
                file.Open(1);
                int size;
                if (Audio_Format_Read_Wave(&file, &format, &size)) {
                    Themes[theme]->Duration = (int)(format.Time_For_Bytes(size) / 1000);
                }
            }
        }
    }
}


// ThemeClass::Base_Name (0x006E7C40): the theme's INI name, "No theme" out of range (TibSun ThemeClass::Base_Name).

char const *ThemeClass::Base_Name(int theme) const
{
    if ((unsigned)theme < (unsigned)Themes.Count()) {
        return Themes[theme]->Name;
    }
    return "No theme";
}


// ThemeClass::ThemeClass() (0x006E7C60)
// Nothing playing or queued, full volume, no themes and no stream.
#ifndef VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#endif
#ifndef VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#endif

ThemeClass::ThemeClass() :
    Current(-1),
    Score(THEME_NONE),
    Pending(THEME_NONE),
    Volume(255),
    IsRepeat(false),
    unknown_11(false),
    IsShuffle(false),
    Sample(0)
{
}


// 0x006e7cb0: function not yet identified
//
// Body: 30 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: int __thiscall FUN_006e7cb0(int param_1,uint param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006e7cb0 {

struct Owner_006e7cb0 { int FUN_006e7cb0(unsigned int theme_index); };

int Owner_006e7cb0::FUN_006e7cb0(unsigned int theme_index)
{
    int theme_address = (int)this;
    if (theme_index < *(unsigned int *)(theme_address + 0x24)) {
        return *(int *)(*(int *)(theme_address + 0x18) + theme_index * 4) + 0x200;
    }
    return 0;
}

}  // namespace seed_006e7cb0


// void ThemeClass::AI() -- 0x006E7CD0 (name hypothesis after the "Theme::AI" debug text)
// Purpose: when sound is enabled and the volume is above zero, lets a playing sample run on (a fading one
// is stopped first once its fade has ended); with nothing playing, starts the queued theme: -2 picks the
// next one (0x006E7D80), -1 and -3 start nothing, and a live-object count other than zero holds the music
// back. The queue is cleared to -2 after the start.


extern char unknown_sound_enabled_a40c18;
extern char unknown_flag_a40d0c;
void __fastcall unknown_stop_sample_407e60(void *sample);
int __fastcall unknown_sample_407f90(void *sample);
int __fastcall unknown_sample_407ff0(void *sample);

void ThemeClass::AI()
{
    if (unknown_check_406f30() && !unknown_flag_a40d0c && unknown_sound_enabled_a40c18 && Volume > 0) {
        bool ready = false;
        if (Sample != 0) {
            if (unknown_11 && !unknown_sample_407ff0(Sample)) {
                unknown_stop_sample_407e60(Sample);
            }
            ready = unknown_sample_407f90(Sample);
        }
        if (ready) {
            return;
        }
        ThemeType theme = Pending;
        if (theme != -1 && theme != -3 && unknown_a40774 == 0) {
            if (theme == -2) {
                Pending = Pick_Next_Song(Score);
                Debug_Printf("Theme::AI(Next song = %d)\n", Pending);
            }
            Play_Song(Pending);
            Pending = -2;
        }
    }
}


// ThemeType ThemeClass::Pick_Next_Song(ThemeType current) (0x006E7D80): the theme after current
// (unless it repeats or is flagged +0x289): with shuffle, up to 1000 random picks of a playable
// other theme (0 when none); otherwise the next playable theme in order (0 after a full cycle).
// Inline element access and count (Themes[current], Themes.Count()).
#ifndef VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#endif
#ifndef VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#endif

ThemeType ThemeClass::Pick_Next_Song(ThemeType current)
{
    if ((current > -1 && !Themes[current]->unknown_289 && !IsRepeat) || current < 0) {
        if (IsShuffle == 1) {
            int tries = 0;
            int pick;
            do {
                pick = unknown_random_839218(0, Themes.Count() - 1);
                tries++;
            } while (tries < 1000 && (pick == current || !unknown_6e8440(pick)));
            if (tries == 1000) {
                pick = 0;
            }
            return pick;
        }
        int remaining = Themes.Count() + 1;
        for (;;) {
            current++;
            if (current >= Themes.Count()) {
                current = 0;
            }
            remaining--;
            if (remaining == 0) {
                return 0;
            }
            if (unknown_6e8440(current)) {
                break;
            }
        }
    }
    return current;
}


// void ThemeClass::Queue_Song(ThemeType theme) -- 0x006E7E20
// Purpose: remembers the theme to play next (+0x08); a real theme other than the current one is logged and,
// while a sample is attached, fades it out (a fade that has ended is stopped first).
// Shape: the fade test is Tiberian Sun's `Still_Playing() == true` (expanded inline), which lets /Ob2
// expand Queue_Song in Play_Song 0x006E7EB0 and Load 0x006E8340.


extern char unknown_sound_enabled_a40c18;
extern char unknown_flag_a40d0c;
void __fastcall unknown_stop_sample_407e60(void *sample);
int __fastcall unknown_sample_407f90(void *sample);
void __fastcall unknown_sample_407fe0(void *sample);
int __fastcall unknown_sample_407ff0(void *sample);

void ThemeClass::Queue_Song(ThemeType theme)
{
    if (unknown_sound_enabled_a40c18 && unknown_check_406f30() && !unknown_flag_a40d0c) {
        Pending = theme;
        if (theme != -1 && theme != -2 && theme != Score) {
            Debug_Printf("Theme::QueueSong(%d)\n", theme);
            if (Still_Playing() == true) {
                if (Sample != 0) {
                    unknown_sample_407fe0(Sample);
                }
                unknown_11 = true;
            }
        }
    }
}


// int ThemeClass::Play_Song(ThemeType theme) -- 0x006E7EB0 (name after Tiberian Sun)
// Purpose: starts `theme` now: the playing theme is stopped first unless it is the same one and still
// playing; a real theme with a volume above zero is opened on the sample (when the CD drive answers,
// or all files are local), otherwise it is queued.


extern char unknown_sound_enabled_a40c18;
extern char unknown_flag_a40d0c;
void __fastcall unknown_stop_sample_407e60(void *sample);
int __fastcall unknown_sample_407f90(void *sample);
void __fastcall unknown_sample_407fe0(void *sample);
int __fastcall unknown_sample_407ff0(void *sample);
struct AudioStreamPlayer;
int __fastcall Audio_Stream_Open(AudioStreamPlayer *stream, char const *name, int raw);

int ThemeClass::Play_Song(ThemeType theme)
{
    if (unknown_sound_enabled_a40c18 && unknown_check_406f30() && !unknown_flag_a40d0c) {
        if (Still_Playing() && Score == theme) {
            return Current;
        }
        Stop();
        unknown_11 = false;
        if (theme != THEME_NONE && theme != -3) {  // -3: Tiberian Sun's THEME_QUIET
            if (theme > THEME_NONE && Volume > 0) {
                Score = theme;
                int cd = CD::IsLocal ? 0 : Unknown_0046E4C0(0);
                if (Sample != 0 && cd > -1) {
                    Audio_Stream_Open((AudioStreamPlayer *)Sample, Theme_File_Name(theme), true);
                    Current = theme;
                } else {
                    Current = THEME_NONE;
                    if (Sample != 0 && cd <= -1 && Pending == THEME_NONE) {
                        Theme.Queue_Song(theme);
                    }
                }
                Debug_Printf("Theme::PlaySong(%d) - %s\n", Score, IsRepeat == true || Themes[theme]->unknown_289 == true ? "Repeating" : "Playing");
                if (IsRepeat == true || Themes[theme]->unknown_289 == true) {
                    Pending = theme;
                }
            } else {
                Pending = theme;
            }
        }
    }
    return Current;
}


// char const *ThemeClass::Theme_File_Name(ThemeType theme) -- 0x006E8110 (name after Tiberian Sun)
// Purpose: the theme's sound file name (Sound + ".WAV") in a static buffer; "" for a theme out of range.

extern "C" void __cdecl _makepath(char *path, char const *drive, char const *dir, char const *fname, char const *ext);

char const *ThemeClass::Theme_File_Name(ThemeType theme)
{
    static char name[256 + 256];

    if ((unsigned)theme < (unsigned)Themes.Count()) {
        _makepath(name, 0, 0, Themes[theme]->Sound, ".WAV");
        return name;
    }
    return "";
}


// void ThemeClass::Stop(bool fade) -- 0x006E81A0
// Purpose: stops the playing theme: with `fade` and a sample attached the sample is faded out when that is
// possible (logged "- Fading"), otherwise it is stopped at once; the current, playing and queued themes
// are forgotten.
// Shape: Tiberian Sun's `fade && Still_Playing() == true` and one chained store keep the body small
// enough for /Ob2 to expand Stop() in Play_Song 0x006E7EB0.


extern char unknown_sound_enabled_a40c18;
extern char unknown_flag_a40d0c;
void __fastcall unknown_stop_sample_407e60(void *sample);
int __fastcall unknown_sample_407f90(void *sample);
void __fastcall unknown_sample_407fe0(void *sample);
int __fastcall unknown_sample_407ff0(void *sample);

void ThemeClass::Stop(bool fade)
{
    if (unknown_sound_enabled_a40c18 && unknown_check_406f30() && !unknown_flag_a40d0c && Current != -1) {
        if (fade && Still_Playing() == true) {
            Debug_Printf("Theme::Stop(%d) - Fading\n", Score);
            if (Sample != 0) {
                unknown_sample_407fe0(Sample);
            }
            unknown_11 = true;
        } else {
            Debug_Printf("Theme::Stop(%d)\n", Score);
            if (Sample != 0) {
                unknown_stop_sample_407e60(Sample);
            }
        }
        Pending = Score = Current = -1;
    }
}


// bool ThemeClass::Still_Playing() const (0x006E82D0)
// The stream is still playing (one that should fade but no longer fades is
// stopped first).

void __fastcall unknown_stop_sample_407e60(void *sample);
int __fastcall unknown_sample_407f90(void *sample);
int __fastcall unknown_sample_407ff0(void *sample);

bool ThemeClass::Still_Playing() const
{
    if (Sample != 0) {
        if (unknown_11 && !unknown_sample_407ff0(Sample)) {
            unknown_stop_sample_407e60(Sample);
        }
        return unknown_sample_407f90(Sample) != 0;
    }
    return false;
}


// long ThemeClass::Load(IStream *stream) -- 0x006E8340 (name hypothesis after Save 0x006E8310)
// Purpose: reads the playing and queued themes from a save stream; when 0x00A40B51 is set the
// playing theme is restarted and the queued one queued again on Theme. Returns the read result.

long ThemeClass::Load(IStream *stream)
{
    ThemeType score;
    ThemeType pending;
    HRESULT hr = stream->Read(&score, sizeof(score), 0);
    if (SUCCEEDED(hr)) {
        hr = stream->Read(&pending, sizeof(pending), 0);
        if (SUCCEEDED(hr) && unknown_flag_a40b51) {
            Theme.Play_Song(score);
            Theme.Queue_Song(pending);
        }
    }
    return hr;
}


// 0x006e8440: function not yet identified
//
// Body: 147 bytes (frozen inventory bound).
// Referenced from 3 sites (unconditional call).
// Ghidra prototype: undefined4 __thiscall FUN_006e8440(int param_1,uint param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// Adjusted from the Ghidra seed by permute_seeds.py (signedness, comparison order, return width) to
// match the original; edit freely, this file is no longer regenerated.

namespace seed_006e8440 {

extern "C" unsigned int DAT_00a35db4;
extern "C" unsigned int Scen_00a3d290;
extern "C" unsigned int Session_00a3d298;

struct Owner_006e8440 { bool FUN_006e8440(unsigned int theme_index); };

bool Owner_006e8440::FUN_006e8440(unsigned int theme_index)
{
    int theme_address = (int)this;
    int theme_entry_address;

    if ((theme_index != 0xfffffffd) && (theme_index != 0xfffffffe)) {
        if (theme_index >= *(unsigned int *)(theme_address + 0x24)) {
            return 0;
        }
        theme_entry_address = (int)(*(int *)(*(int *)(theme_address + 0x18) + theme_index * 4));
        if (*(char *)(theme_entry_address + 0x28a) == '\0') {
            return 0;
        }
        if (*(char *)(theme_entry_address + 0x288) == '\0') {
            return 0;
        }
        if (((DAT_00a35db4 != 0) && (*(int *)(theme_entry_address + 0x28c) != -1)) &&
              (*(int *)(*(int *)(DAT_00a35db4 + 0x34) + 0xbc) != *(int *)(theme_entry_address + 0x28c))) {
            return 0;
        }
        if ((Session_00a3d298 == 0) && (*(int *)(Scen_00a3d290 + 0x834) < *(int *)(theme_entry_address + 0x280))) {
            return 0;
        }
    }
    return 1;
}

}  // namespace seed_006e8440


// int ThemeClass::From_Name(char const *name) const (0x006E8510)
// The theme whose INI name is name (case blind), or -1.
#ifndef VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#endif
#ifndef VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#endif

extern "C" unsigned int __cdecl strlen(char const *string);
#pragma intrinsic(strlen)
extern "C" int __cdecl _stricmp(char const *left, char const *right);

int ThemeClass::From_Name(char const *name) const
{
    if (name != 0 && strlen(name) > 0) {
        for (int theme = 0; theme < Themes.Count(); theme++) {
            if (_stricmp(Themes[theme]->Name, name) == 0) {
                return theme;
            }
        }
    }
    return THEME_NONE;
}


// ThemeClass::Name (0x006E8570): the theme's INI name, "none" out of range.

char const *ThemeClass::Name(int theme) const
{
    if (theme >= 0 && theme < Themes.Count()) {
        return Themes[theme]->Name;
    }
    return "none";
}


// void ThemeClass::Set_Volume(int volume) (0x006E8590)
// The theme volume, at most 255.

void ThemeClass::Set_Volume(int volume)
{
    if (volume >= 255) {
        volume = 255;
    }
    Volume = volume;
}


extern "C" unsigned char vtable_6e86e0[]; extern "C" void __cdecl release_buffer_6e86e0(void*); extern "C" void __cdecl release_object_6e86e0(void*);
class Leaf_6e86e0 { public: void* get(unsigned int flags); };
void* Leaf_6e86e0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6e86e0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6e86e0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6e86e0(this); return this; }
