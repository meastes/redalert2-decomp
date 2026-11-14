// vox.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/vox.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x00715480 and the
// unit's VoxClass list; the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

class VoxClass;

// The unit's list of VoxClass objects (0x00AD0930; name is a hypothesis).
DynamicVectorClass<VoxClass *> unknown_VoxClass_list_00ad0930;


// Dynamic initializer (0x007154B0, _$E) emitted by HeaderStatics_diagonal_init_00715480.cpp; no source of its own.


// Dynamic initializer (0x007154D0, _$E) emitted by HeaderStatics_diagonal_init_00715480.cpp; no source of its own.


// Dynamic initializer (0x007154F0, _$E) emitted by HeaderStatics_diagonal_init_00715480.cpp; no source of its own.


// Dynamic initializer (0x00715510, _$E) emitted by HeaderStatics_diagonal_init_00715480.cpp; no source of its own.


// Dynamic initializer (0x00715530, _$E) emitted by HeaderStatics_diagonal_init_00715480.cpp; no source of its own.


// Dynamic initializer (0x00715550, _$E) emitted by HeaderStatics_diagonal_init_00715480.cpp; no source of its own.


// Dynamic initializer (0x00715590, _$E) emitted by HeaderStatics_diagonal_init_00715480.cpp; no source of its own.


// Dynamic initializer (0x007155C0, _$E) emitted by HeaderStatics_diagonal_init_00715480.cpp; no source of its own.


// Dynamic initializer (0x007155F0, _$E) emitted by HeaderStatics_diagonal_init_00715480.cpp; no source of its own.


// Dynamic initializer (0x00715620, _$E) emitted by HeaderStatics_diagonal_init_00715480.cpp; no source of its own.


// Dynamic initializer (0x00715630, _$E) emitted by HeaderStatics_diagonal_init_00715480.cpp; no source of its own.


// Dynamic initializer (0x00715650, _$E) emitted by HeaderStatics_diagonal_init_00715480.cpp; no source of its own.


// Dynamic initializer (0x00715690, _$E) emitted by HeaderStatics_diagonal_init_00715480.cpp; no source of its own.


// int Speech_Init() (0x007156D0): empties the speech lists, enables speech and
// makes the "EVA Stream" stream with the global levels 1 and 5 (0 without a
// stream; name hypothesis).
#include "shared/vox_speech.hpp"

int Speech_Init()
{
    unknown_vox_ad0878 = 0;
    unknown_countdown_ad08c8 = 0;
    for (int i = 0; i < 4; i++) {
        Audio_List_Init(&unknown_vox_lists_ad08e0[i]);
    }
    Audio_List_Init(&unknown_vox_list_ad0890);
    Audio_List_Init(&unknown_vox_list_ad0868);
    unknown_vox_pending_ad0948 = 0;
    unknown_vox_enabled_ad094c = 1;
    if (unknown_vox_stream_ad095c == 0) {
        unknown_vox_stream_ad095c = Audio_Stream_Create(Audio_Get_Device(), 3000);
        if (unknown_vox_stream_ad095c == 0) {
            return 0;
        }
    }
    Audio_Stream_Set_Name(unknown_vox_stream_ad095c, "EVA Stream");
    Audio_Stream_Set_Levels_94(unknown_vox_stream_ad095c, Audio_Global_Levels[1]);
    Audio_Stream_Set_Levels_9c(unknown_vox_stream_ad095c, Audio_Global_Levels[5]);
    return 1;
}


// void Speech_Shutdown() (0x00715780): clears the speech queue, then stops and
// destroys the stream (name hypothesis).

void Speech_Shutdown()
{
    Speech_Clear();
    if (unknown_vox_stream_ad095c != 0) {
        Audio_Stream_Stop(unknown_vox_stream_ad095c);
        Audio_Stream_Destroy(unknown_vox_stream_ad095c);
        unknown_vox_stream_ad095c = 0;
    }
}


// void __fastcall Speech_Clear() (0x007157B0): drops every waiting speech, marking its
// VoxClass idle (name hypothesis).

void __fastcall Speech_Clear()
{
    if (!unknown_vox_enabled_ad094c) {
        return;
    }
    if (unknown_vox_pending_ad0948 != 0) {
        unknown_vox_pending_ad0948->Vox->State = 2;
        delete unknown_vox_pending_ad0948;
        unknown_vox_pending_ad0948 = 0;
    }
    VoxSpeech *speech;
    while ((speech = (VoxSpeech *)Audio_List_First(&unknown_vox_list_ad0890)) != 0) {
        Audio_List_Remove(speech);
        speech->Vox->State = 2;
        delete speech;
    }
    while ((speech = (VoxSpeech *)Audio_List_First(&unknown_vox_list_ad0868)) != 0) {
        Audio_List_Remove(speech);
        speech->Vox->State = 2;
        delete speech;
    }
    for (int i = 0; i < 4; i++) {
        while ((speech = (VoxSpeech *)Audio_List_First(&unknown_vox_lists_ad08e0[i])) != 0) {
            Audio_List_Remove(speech);
            speech->Vox->State = 2;
            delete speech;
        }
    }
}


// char const *VoxClass::Name_From(int index) (0x007158A0): the name of the
// VoxClass at an index, "none" out of range (name hypothesis).

char const *__fastcall VoxClass::Name_From(int index)
{
    if (index != -1 && index >= 0 && index < unknown_VoxClass_list_00ad0930.Count()) {
        return unknown_VoxClass_list_00ad0930[index]->Name;
    }
    return "none";
}


// void Speech_Play(int index, int type, int priority) (0x007158C0): plays the
// VoxClass at an index unless it is playing already; type 2 drops the type 3
// speeches and stops the speech playing first (name hypothesis).

void __fastcall Speech_Play(int index, int type, int priority)
{
    if (unknown_vox_stream_ad095c != 0 && index >= 0 && index < unknown_VoxClass_list_00ad0930.Count()
            && unknown_vox_ad0878 == 0) {
        VoxClass *vox = unknown_VoxClass_list_00ad0930[index];
        if (vox != unknown_vox_playing_ad0954) {
            if (type == -1) {
                type = vox->Type;
            }
            if (priority == -1) {
                priority = vox->Priority;
            }
            if (unknown_vox_playing_ad0954 != 0 && type == 2) {
                VoxSpeech *speech;
                while ((speech = (VoxSpeech *)Audio_List_First(&unknown_vox_list_ad0868)) != 0) {
                    Audio_List_Remove(speech);
                    speech->Vox->State = 2;
                    delete speech;
                }
                Speech_Stop(true);
                unknown_vox_delay_ad0960 = 0;
            }
            VoxSpeech *speech = Speech_Find(vox);
            if (speech == 0 || speech->Type != type) {
                Speech_Queue(vox, type, priority);
            }
            Speech_Update();
        }
    }
}


// void Speech_Queue(VoxClass *vox, int type, int priority) (0x007159D0): a new
// speech node: type 3 and type 1 (by priority) and priority 3 speeches wait
// in their lists; any other is kept as the one pending speech when nothing
// else waits and it outranks the pending one, else dropped (name hypothesis).

void __fastcall Speech_Queue(VoxClass *vox, int type, int priority)
{
    VoxSpeech *speech = new VoxSpeech;
    Audio_List_Init_Node(speech);
    speech->Vox = vox;
    speech->Priority = priority;
    speech->Type = type;
    speech->Serial = unknown_vox_serial_ad0950 % 100;
    unknown_vox_serial_ad0950++;
    vox->State = 1;
    if (type == 3) {
        Audio_List_Add_Tail(&unknown_vox_list_ad0868, speech);
    } else if (type == 1) {
        Audio_List_Add_Tail(&unknown_vox_lists_ad08e0[priority], speech);
    } else if (priority == 3) {
        Audio_List_Add_Tail(&unknown_vox_list_ad0890, speech);
    } else if (unknown_vox_list_ad0868.Prev == &unknown_vox_list_ad0868
            && unknown_vox_list_ad0890.Prev == &unknown_vox_list_ad0890
            && (unknown_vox_pending_ad0948 == 0 || unknown_vox_pending_ad0948->Priority < priority)) {
        unknown_vox_pending_ad0948 = speech;
    } else {
        vox->State = 2;
        delete speech;
    }
}


// VoxSpeech *Speech_Find(VoxClass *vox) (0x00715AC0): the waiting speech of a
// VoxClass: the priority 3 list, the pending speech, the type 1 lists from
// priority 3 down, then the type 3 list; 0 for none (name hypothesis).

VoxSpeech *__fastcall Speech_Find(VoxClass *vox)
{
    VoxSpeech *speech;
    for (speech = (VoxSpeech *)Audio_List_First(&unknown_vox_list_ad0890); speech != 0;
            speech = (VoxSpeech *)Audio_List_Next(speech)) {
        if (speech->Vox == vox) {
            return speech;
        }
    }
    if (unknown_vox_pending_ad0948 != 0 && unknown_vox_pending_ad0948->Vox == vox) {
        return unknown_vox_pending_ad0948;
    }
    for (int priority = 3; priority >= 0; priority--) {
        for (speech = (VoxSpeech *)Audio_List_First(&unknown_vox_lists_ad08e0[priority]); speech != 0;
                speech = (VoxSpeech *)Audio_List_Next(speech)) {
            if (speech->Vox == vox) {
                return speech;
            }
        }
    }
    for (speech = (VoxSpeech *)Audio_List_First(&unknown_vox_list_ad0868); speech != 0;
            speech = (VoxSpeech *)Audio_List_Next(speech)) {
        if (speech->Vox == vox) {
            return speech;
        }
    }
    return 0;
}


// void Speech_Play_Name(char const *name, int type, int priority) (0x00715B40):
// plays the VoxClass of a name; nothing for a null name (name hypothesis).
void __fastcall Speech_Play_Name(char const *name, int type, int priority)
{
    if (name != 0) {
        Speech_Play(VoxClass::Find_Index(name), type, priority);
    }
}


// void Speech_Update() (0x00715BA0): when the stream is free and the gap after the
// last speech has passed, starts the next waiting speech: a type 3 one, a
// priority 3 one, the pending one, then the first of the type 1 lists from
// priority 3 down. Its VoxClass's file (".WAV") is opened on the stream (name
// hypothesis).
#include "shared/sound_available.hpp"
#include "shared/crt_string.hpp"

extern char unknown_flag_a40d0c;

void Speech_Update()
{
    if (unknown_flag_a40d0c || !unknown_check_406f30() || unknown_vox_stream_ad095c == 0
            || Audio_Stream_Is_Playing(unknown_vox_stream_ad095c)) {
        return;
    }
    if (Audio_Now() <= Audio_Stream_Stop_Time(unknown_vox_stream_ad095c) + unknown_vox_delay_ad0960) {
        return;
    }
    if (unknown_countdown_ad08c8 != 0) {
        return;
    }
    if (unknown_vox_playing_ad0954 != 0) {
        unknown_vox_playing_ad0954->State = 2;
        unknown_vox_playing_ad0954 = 0;
    }
    int type = 0;
    int priority = 0;
    VoxSpeech *speech = (VoxSpeech *)Audio_List_First(&unknown_vox_list_ad0868);
    if (speech != 0) {
        Audio_List_Remove(speech);
        if (unknown_vox_pending_ad0948 != 0) {
            delete unknown_vox_pending_ad0948;
            unknown_vox_pending_ad0948 = 0;
        }
    } else {
        speech = (VoxSpeech *)Audio_List_First(&unknown_vox_list_ad0890);
        if (speech != 0) {
            Audio_List_Remove(speech);
            if (unknown_vox_pending_ad0948 != 0) {
                delete unknown_vox_pending_ad0948;
                unknown_vox_pending_ad0948 = 0;
            }
        } else if (unknown_vox_pending_ad0948 != 0) {
            speech = unknown_vox_pending_ad0948;
            unknown_vox_pending_ad0948 = 0;
        } else {
            for (int i = 3; i >= 0; i--) {
                speech = (VoxSpeech *)Audio_List_First(&unknown_vox_lists_ad08e0[i]);
                if (speech != 0) {
                    break;
                }
            }
            if (speech != 0) {
                Audio_List_Remove(speech);
            }
        }
    }
    if (speech == 0) {
        return;
    }
    VoxClass *vox = speech->Vox;
    speech->Get(&type, &priority);
    delete speech;
    if (vox == 0) {
        return;
    }
    vox->State = 2;
    char filename[256];
    strcpy(filename, unknown_flag_ad0958 == 0 ? vox->Allied : vox->Russian);
    strcat(filename, ".WAV");
    if (Audio_Stream_Open(unknown_vox_stream_ad095c, filename, 1)) {
        unknown_vox_delay_ad0960 = 500;
        unknown_vox_playing_ad0954 = vox;
        unknown_vox_type_ad0858 = type;
        unknown_vox_priority_ad0880 = priority;
        vox->State = 0;
    }
}


// void Speech_Stop(bool clear) (0x00715DE0): stops the speech playing (its
// VoxClass idle again) and, with clear set, drops the waiting speeches too
// (name hypothesis).

void __fastcall Speech_Stop(bool clear)
{
    if (unknown_vox_playing_ad0954 != 0) {
        unknown_vox_playing_ad0954->State = 2;
        unknown_vox_playing_ad0954 = 0;
    }
    if (unknown_vox_stream_ad095c != 0) {
        Audio_Stream_Stop(unknown_vox_stream_ad095c);
    }
    if (clear) {
        Speech_Clear();
    }
}


// // Whether no speech waits (inline).
static inline bool Speech_Lists_Empty()
{
    if (unknown_vox_list_ad0868.Prev != &unknown_vox_list_ad0868
            || unknown_vox_list_ad0890.Prev != &unknown_vox_list_ad0890
            || unknown_vox_pending_ad0948 != 0) {
        return false;
    }
    for (int i = 0; i < 4; i++) {
        AudioListNode *list = &unknown_vox_lists_ad08e0[i];
        if (list->Prev != list) {
            return false;
        }
    }
    return true;
}

bool Speech_Is_Playing()
{
    Speech_Update();
    return unknown_vox_stream_ad095c != 0
            && (Audio_Stream_Is_Playing(unknown_vox_stream_ad095c) || !Speech_Lists_Empty());
}


// void Speech_Cancel(int index) (0x00715E80): drops every waiting speech of
// the VoxClass at an index, marking it idle (name hypothesis).

void __fastcall Speech_Cancel(int index)
{
    if (index >= 0 && index < unknown_VoxClass_list_00ad0930.Count()) {
        VoxSpeech *speech;
        while ((speech = Speech_Find(unknown_VoxClass_list_00ad0930[index])) != 0) {
            if (speech->Prev != speech) {
                Audio_List_Remove(speech);
            }
            if (speech == unknown_vox_pending_ad0948) {
                unknown_vox_pending_ad0948 = 0;
            }
            speech->Vox->State = 2;
            delete speech;
        }
    }
}


// void VoxClass::Set_Volume(int volume) (0x00715EF0): the speech volume, at
// most 255.

void __fastcall VoxClass::Set_Volume(int volume)
{
    unknown_vox_volume_7f9bd4 = volume;
    if (volume >= 255) {
        unknown_vox_volume_7f9bd4 = 255;
    }
}


// int Taunt_Init() (0x00715F10): makes the "Taunt Stream" stream (0 without
// one); the global levels 1 and 5 are set on the speech stream (name
// hypothesis).

int Taunt_Init()
{
    if (unknown_taunt_stream_ad0968 == 0) {
        unknown_taunt_stream_ad0968 = Audio_Stream_Create(Audio_Get_Device(), 3000);
        if (unknown_taunt_stream_ad0968 == 0) {
            return 0;
        }
    }
    Audio_Stream_Set_Name(unknown_taunt_stream_ad0968, "Taunt Stream");
    Audio_Stream_Set_Levels_94(unknown_vox_stream_ad095c, Audio_Global_Levels[1]);
    Audio_Stream_Set_Levels_9c(unknown_vox_stream_ad095c, Audio_Global_Levels[5]);
    unknown_count_ad0910 = 0;
    return 1;
}


// void Taunt_Shutdown() (0x00715F80): stops and destroys the taunt stream
// (name hypothesis).

void Taunt_Shutdown()
{
    if (unknown_taunt_stream_ad0968 != 0) {
        Audio_Stream_Stop(unknown_taunt_stream_ad0968);
        Audio_Stream_Destroy(unknown_taunt_stream_ad0968);
        unknown_taunt_stream_ad0968 = 0;
    }
}


// int Taunt_Play(int taunt) (0x00715FB0): plays taunt number 1..8 (low four
// bits) of a side (next four bits, 0..8) on the taunt stream (name hypothesis).
extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);

int __fastcall Taunt_Play(int taunt)
{
    char name[20];
    if (unknown_taunt_stream_ad0968 == 0 || unknown_count_ad0910 != 0) {
        return 0;
    }
    char number = (char)(taunt & 0xf);
    if (number < 1 || number > 8) {
        return 0;
    }
    switch ((taunt >> 4) & 0xf) {
    case 0:
        sprintf(name, "taunts\\tauam%02i.wav", number);
        break;
    case 1:
        sprintf(name, "taunts\\tauko%02i.wav", number);
        break;
    case 2:
        sprintf(name, "taunts\\taufr%02i.wav", number);
        break;
    case 3:
        sprintf(name, "taunts\\tauge%02i.wav", number);
        break;
    case 4:
        sprintf(name, "taunts\\taubr%02i.wav", number);
        break;
    case 5:
        sprintf(name, "taunts\\tauli%02i.wav", number);
        break;
    case 6:
        sprintf(name, "taunts\\tauir%02i.wav", number);
        break;
    case 7:
        sprintf(name, "taunts\\taucu%02i.wav", number);
        break;
    case 8:
        sprintf(name, "taunts\\tauru%02i.wav", number);
        break;
    default:
        return 0;
    }
    return Audio_Stream_Open(unknown_taunt_stream_ad0968, name, 0);
}


// void Unknown_007160D0() (0x007160D0): stops the taunt stream.

void __fastcall Unknown_007160D0()
{
    if (unknown_taunt_stream_ad0968 != 0) {
        Audio_Stream_Stop(unknown_taunt_stream_ad0968);
    }
}


// VoxClass::VoxClass(char const *name) (0x007160E0): a speech of a name at
// full volume, priority 1, type 0 and idle, added to the unit's VoxClass list.

VoxClass::VoxClass(char const *name)
{
    Volume = 1.0f;
    strcpy(Name, name);
    Russian[0] = '\0';
    Allied[0] = '\0';
    Priority = 1;
    Type = 0;
    State = 2;
    unknown_VoxClass_list_00ad0930.Add(this);
}


// VoxClass::~VoxClass() (0x00716190): leaves the unit's VoxClass list.

VoxClass::~VoxClass()
{
    unknown_VoxClass_list_00ad0930.Delete(this);
}


// bool VoxClass::Read_INI(CCINIClass &ini) (0x007161E0): the volume, type
// (QUEUE 1, STANDARD 0, INTERRUPT 2, QUEUED_INTERRUPT 3), priority (LOW 0,
// NORMAL 1, IMPORTANT 2, CRITICAL 3) and the Russian and Allied file names of
// the VoxClass's EVA.ini section; false without the section.
#include "shared/ccini_class.hpp"

bool VoxClass::Read_INI(CCINIClass &ini)
{
    char buffer[500];
    Volume = 1.0f;
    if (ini.Is_Present(Name)) {
        Volume = (float)ini.Get_Double(Name, "Volume", Volume);
        ini.Get_String(Name, "Type", "", buffer, sizeof(buffer));
        if (strlen(buffer) != 0) {
            if (_stricmp(buffer, "QUEUE") == 0) {
                Type = 1;
            } else if (_stricmp(buffer, "STANDARD") == 0) {
                Type = 0;
            } else if (_stricmp(buffer, "INTERRUPT") == 0) {
                Type = 2;
            } else if (_stricmp(buffer, "QUEUED_INTERRUPT") == 0) {
                Type = 3;
            }
        }
        ini.Get_String(Name, "Priority", "", buffer, sizeof(buffer));
        if (strlen(buffer) != 0) {
            if (_stricmp(buffer, "LOW") == 0) {
                Priority = 0;
            } else if (_stricmp(buffer, "NORMAL") == 0) {
                Priority = 1;
            } else if (_stricmp(buffer, "IMPORTANT") == 0) {
                Priority = 2;
            } else if (_stricmp(buffer, "CRITICAL") == 0) {
                Priority = 3;
            }
        }
        ini.Get_String(Name, "Russian", "", buffer, sizeof(buffer));
        strncpy(Russian, buffer, sizeof(Russian));
        Russian[sizeof(Russian) - 1] = '\0';
        ini.Get_String(Name, "Allied", "", buffer, sizeof(buffer));
        strncpy(Allied, buffer, sizeof(Allied));
        Allied[sizeof(Allied) - 1] = '\0';
        return true;
    }
    return false;
}


// void VoxClass::Process(CCINIClass &ini) (0x007163F0): makes (or finds) a
// VoxClass for each [DialogList] entry of EVA.ini, which then reads its own
// section (name hypothesis).
void __fastcall VoxClass::Process(CCINIClass &ini)
{
    if (ini.Is_Present("DialogList")) {
        int count = ini.Entry_Count("DialogList");
        for (int index = 0; index < count; index++) {
            char name[200];
            if (ini.Get_String("DialogList", ini.Get_Entry("DialogList", index), "", name, sizeof(name))) {
                VoxClass *vox;
                int vox_index = Find_Index(name);
                if (vox_index == -1) {
                    vox = new VoxClass(name);
                } else {
                    vox = unknown_VoxClass_list_00ad0930[vox_index];
                }
                vox->Read_INI(ini);
            }
        }
    }
}


// void Speech_Free_All() (0x00716590): stops the speech, drops the waiting
// ones and deletes every VoxClass (name hypothesis).

void Speech_Free_All()
{
    Speech_Stop(true);
    while (unknown_VoxClass_list_00ad0930.Count() > 0) {
        delete unknown_VoxClass_list_00ad0930[0];
    }
}


// int VoxClass::Find_Index(char const *name) (0x00716640): the index of the
// VoxClass of a name, -1 for none or a null name.
#include "shared/vox_class.hpp"

// The list search after the null test is a separate inline step: the original
// keeps the null test's -1 return before the search saves esi (one function
// body merges both -1 returns and saves esi at entry).
static inline int Index_Of_Name(char const *name)
{
    for (int index = 0; index < unknown_VoxClass_list_00ad0930.Count(); index++) {
        if (_stricmp(name, unknown_VoxClass_list_00ad0930[index]->Name) == 0) {
            return index;
        }
    }
    return -1;
}

int __fastcall VoxClass::Find_Index(char const *name)
{
    if (name == 0) {
        return -1;
    }
    return Index_Of_Name(name);
}


// 0x00716690: function not yet identified
//
// Body: 33 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: bool __fastcall FUN_00716690(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// Adjusted from the Ghidra seed by permute_seeds.py (signedness, comparison order) to
// match the original; edit freely, this file is no longer regenerated.

namespace seed_00716690 {

extern "C" unsigned int DAT_00ad0934;
extern "C" int DAT_00ad0940;

bool __fastcall FUN_00716690(int voice_index)
{
    if ((voice_index >= 0) && (voice_index < DAT_00ad0940)) {
        return *(int *)(*(int *)(DAT_00ad0934 + voice_index * 4) + 0x48) == 2;
    }
    return true;
}

}  // namespace seed_00716690


// VoxClass *VoxClass::Find(char const *name) (0x007166C0): the VoxClass of a
// name; 0 for a null name, "<none>" or no match.

VoxClass *__fastcall VoxClass::Find(char const *name)
{
    if (name != 0 && _stricmp(name, "<none>") != 0) {
        for (int index = 0; index < unknown_VoxClass_list_00ad0930.Count(); index++) {
            if (_stricmp(name, unknown_VoxClass_list_00ad0930[index]->Name) == 0) {
                return unknown_VoxClass_list_00ad0930[index];
            }
        }
    }
    return 0;
}


// 0x00716720: function not yet identified
//
// Body: 28 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: char * __fastcall FUN_00716720(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// Adjusted from the Ghidra seed by permute_seeds.py (signedness, comparison order, return width) to
// match the original; edit freely, this file is no longer regenerated.

namespace seed_00716720 {

extern "C" unsigned int DAT_00ad0934;
extern "C" int DAT_00ad0940;
extern "C" char s__none__007cdecc[];

char * __fastcall FUN_00716720(int voice_index)
{
    if ((voice_index != -1) && (voice_index < DAT_00ad0940)) {
        return *(char **)(DAT_00ad0934 + voice_index * 4);
    }
    return s__none__007cdecc;
}

}  // namespace seed_00716720


// int VoxClass::Index_Of(VoxClass *vox) (0x00716740): the index of a VoxClass
// in the unit's list, -1 for none.

int __fastcall VoxClass::Index_Of(VoxClass *vox)
{
    for (int index = 0; index < unknown_VoxClass_list_00ad0930.Count(); index++) {
        if (unknown_VoxClass_list_00ad0930[index] == vox) {
            return index;
        }
    }
    return -1;
}


// void *UnknownOwner_00716770::unknown_select() -- 0x00716770
// Purpose: the member at +0x35 normally, the one at +0x2C while the flag
// at 0x00AD0958 is set.
// Assumptions: owner and members unidentified (sound area).

extern int unknown_flag_ad0958;

struct UnknownOwner_00716770 {
    char unknown_00[0x2C];
    char unknown_2c[0x35 - 0x2C];           // +0x2C
    char unknown_35[1];                     // +0x35
    void *unknown_select();
};

void *UnknownOwner_00716770::unknown_select()
{
    return unknown_flag_ad0958 == 0 ? unknown_35 : unknown_2c;
}


// long Speech_Load(IStream *stream) (0x007167D0): stops the speech, then
// queues the speeches a saved game lists between 'SxoV' and 'ExoV' (each an
// 'IxoV' marker and the VoxClass index, type and priority); E_FAIL for an
// unknown marker (name hypothesis).
#include "shared/com_persist.hpp"

long __fastcall Speech_Load(IStream *stream)
{
    Speech_Stop(true);
    int marker;
    HRESULT result = stream->Read(&marker, sizeof(marker), 0);
    if (FAILED(result) || marker != 0x566F7853) {
        return result;
    }
    for (result = stream->Read(&marker, sizeof(marker), 0); SUCCEEDED(result);
            result = stream->Read(&marker, sizeof(marker), 0)) {
        if (marker == 0x566F7845) {
            break;
        }
        if (marker != 0x566F7849) {
            return (HRESULT)0x80004005L;
        }
        struct {
            int Index;
            int Type;
            int Priority;
        } entry;
        result = stream->Read(&entry, sizeof(entry), 0);
        if (FAILED(result)) {
            break;
        }
        if (unknown_vox_stream_ad095c != 0 && entry.Index >= 0 && entry.Index < unknown_VoxClass_list_00ad0930.Count()) {
            Speech_Queue(unknown_VoxClass_list_00ad0930[entry.Index], entry.Type, entry.Priority);
        }
    }
    return result;
}


#include "shared/unknown_globals.hpp"


// void __fastcall Unknown_007168C0(int side) (0x007168C0)
// Records the side whose speech files EVA plays (0x00AD0958).

void __fastcall Unknown_007168C0(int side)
{
    unknown_flag_ad0958 = side;
}


// void Taunt_Pause() (0x007168D0): pauses the taunt stream.

void Taunt_Pause()
{
    if (unknown_taunt_stream_ad0968 != 0) {
        Audio_Stream_Pause(unknown_taunt_stream_ad0968);
    }
}


// void Taunt_Resume() (0x007168E0): resumes the taunt stream.

void Taunt_Resume()
{
    if (unknown_taunt_stream_ad0968 != 0) {
        Audio_Stream_Resume(unknown_taunt_stream_ad0968);
    }
}


// void Taunt_Unblock() (0x00716910): counts the taunt block down, not below 0.

void Taunt_Unblock()
{
    if (unknown_taunt_stream_ad0968 != 0) {
        if (--unknown_count_ad0910 < 0) {
            unknown_count_ad0910 = 0;
        }
    }
}


class Leaf_716940 { public: void get(); };
void Leaf_716940::get() { unsigned int unused_this_address = (unsigned int)this; unsigned int counter_value; unsigned int unused_value; counter_value = *(unsigned int*)(0xad0878); counter_value = (counter_value) + 1; *(unsigned int*)(0xad0878) = counter_value; }


// void Unknown_00716950() (0x00716950): counts the speech block down, not
// below 0.

void Unknown_00716950()
{
    if (unknown_vox_ad0878 != 0) {
        if (--unknown_vox_ad0878 < 0) {
            unknown_vox_ad0878 = 0;
        }
    }
}


// void Speech_Hold() (0x00716980): pauses the speech stream and blocks new
// speeches from starting (name hypothesis).

void Speech_Hold()
{
    if (unknown_vox_stream_ad095c != 0) {
        Audio_Stream_Pause(unknown_vox_stream_ad095c);
    }
    unknown_countdown_ad08c8++;
}


// void Speech_Reset() (0x007169A0): stops the speech, drops the waiting ones
// and clears both speech blocks (name hypothesis).

void Speech_Reset()
{
    Speech_Stop(true);
    unknown_countdown_ad08c8 = 0;
    unknown_vox_ad0878 = 0;
}


// void unknown_tick_sound() -- 0x007169F0
// Purpose: runs 0x00407F20 on the object at 0x00AD095C when there is one,
// then counts 0x00AD08C8 down to 0.
// Assumptions: sound area; names are hypotheses.

struct UnknownSound_007169f0 {
    void unknown_call_407f20();             // 0x00407F20
};
extern UnknownSound_007169f0 *unknown_object_ad095c;
extern volatile int unknown_countdown_ad08c8;

void unknown_tick_sound()
{
    if (unknown_object_ad095c != 0) {
        unknown_object_ad095c->unknown_call_407f20();
    }
    if (unknown_countdown_ad08c8 != 0) {
        if (--unknown_countdown_ad08c8 < 0) {
            unknown_countdown_ad08c8 = 0;
        }
    }
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<VoxClass *>::Clear (0x00716A20; DynamicVectorClass<VoxClass *> vtable 0x007ae9e4 slot 3).
// DynamicVectorClass<VoxClass *>::ID_pointer (0x00716A50; DynamicVectorClass<VoxClass *> vtable 0x007ae9e4 slot 5).
// VectorClass<VoxClass *>::Fetch (0x00716A70; VectorClass<VoxClass *> vtable 0x007aea04 slot 6).
// VectorClass<VoxClass *>::operator equal (0x00716A80; VectorClass<VoxClass *> vtable 0x007aea04 slot 1).
// VectorClass<VoxClass *>::Resize (0x00716AC0; VectorClass<VoxClass *> vtable 0x007aea04 slot 2).
// VectorClass<VoxClass *>::Clear (0x00716B70; VectorClass<VoxClass *> vtable 0x007aea04 slot 3).
// VectorClass<VoxClass *>::ID_object (0x00716BA0; VectorClass<VoxClass *> vtable 0x007aea04 slot 4).
// VectorClass<VoxClass *>::ID_pointer (0x00716BE0; VectorClass<VoxClass *> vtable 0x007aea04 slot 5).
// DynamicVectorClass<VoxClass *>::Resize (0x00716C00; DynamicVectorClass<VoxClass *> vtable 0x007ae9e4 slot 2).
// DynamicVectorClass<VoxClass *>::ID_object (0x00716CB0; DynamicVectorClass<VoxClass *> vtable 0x007ae9e4 slot 4).


extern "C" unsigned char vtable_716ce0[]; extern "C" void __cdecl release_buffer_716ce0(void*); extern "C" void __cdecl release_object_716ce0(void*);
class Leaf_716ce0 { public: void* get(unsigned int flags); };
void* Leaf_716ce0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_716ce0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_716ce0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_716ce0(this); return this; }


extern "C" unsigned char vtable_716d30[]; extern "C" void __cdecl release_buffer_716d30(void*); extern "C" void __cdecl release_object_716d30(void*);
class Leaf_716d30 { public: void* get(unsigned int flags); };
void* Leaf_716d30::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_716d30; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_716d30((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_716d30(this); return this; }
