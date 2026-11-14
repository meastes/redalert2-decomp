// voc.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/voc.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x007135A0 and the
// unit's VocClass list; the other drafts of this plan are emitted by this file.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

class VocClass;

// The unit's list of VocClass objects (0x00AD0818; name is a hypothesis).
DynamicVectorClass<VocClass *> unknown_VocClass_list_00ad0818;


// Dynamic initializer (0x007135C0, _$E) emitted by HeaderStatics_empty_rect_init_007135a0.cpp; no source of its own.


// Dynamic initializer (0x007135F0, _$E) emitted by HeaderStatics_empty_rect_init_007135a0.cpp; no source of its own.


// Dynamic initializer (0x00713610, _$E) emitted by HeaderStatics_empty_rect_init_007135a0.cpp; no source of its own.


// Dynamic initializer (0x00713630, _$E) emitted by HeaderStatics_empty_rect_init_007135a0.cpp; no source of its own.


// Dynamic initializer (0x00713650, _$E) emitted by HeaderStatics_empty_rect_init_007135a0.cpp; no source of its own.


// Dynamic initializer (0x00713670, _$E) emitted by HeaderStatics_empty_rect_init_007135a0.cpp; no source of its own.


// Dynamic initializer (0x00713690, _$E) emitted by HeaderStatics_empty_rect_init_007135a0.cpp; no source of its own.


// Dynamic initializer (0x007136D0, _$E) emitted by HeaderStatics_empty_rect_init_007135a0.cpp; no source of its own.


// Dynamic initializer (0x00713700, _$E) emitted by HeaderStatics_empty_rect_init_007135a0.cpp; no source of its own.


// Dynamic initializer (0x00713730, _$E) emitted by HeaderStatics_empty_rect_init_007135a0.cpp; no source of its own.


// Dynamic initializer (0x00713760, _$E) emitted by HeaderStatics_empty_rect_init_007135a0.cpp; no source of its own.


// Dynamic initializer (0x00713770, _$E) emitted by HeaderStatics_empty_rect_init_007135a0.cpp; no source of its own.


// Dynamic initializer (0x00713790, _$E) emitted by HeaderStatics_empty_rect_init_007135a0.cpp; no source of its own.


// Dynamic initializer (0x007137D0, _$E) emitted by HeaderStatics_empty_rect_init_007135a0.cpp; no source of its own.


// VocClass::VocClass(char const *name) (0x00713810): joins the unit's VocClass
// list, then looks up the audio event type of the name.
#include "shared/voc_class.hpp"

VocClass::VocClass(char const *name) : Event(0)
{
    unknown_VocClass_list_00ad0818.Add(this);
    Event = Audio_Event_Type_Find(name);
}


// VocClass::~VocClass() (0x00713880): leaves the unit's VocClass list.

VocClass::~VocClass()
{
    unknown_VocClass_list_00ad0818.Delete(this);
}


// bool VocClass::Read_INI(CCINIClass &ini) (0x007138D0): sets the event type's
// samples and settings from its Sound.ini section; false without an event type
// or section.
#include "shared/ccini_class.hpp"
#include "shared/crt_tokens.hpp"
extern "C" int __cdecl atoi(char const *string);

bool VocClass::Read_INI(CCINIClass &ini)
{
    if (Event == 0 || !ini.Is_Present(Name())) {
        return false;
    }
    char buffer[0x800];
    char *token;

    ini.Get_String(Name(), "Sounds", "", buffer, sizeof(buffer));
    for (token = strtok(buffer, " \t\n"); token != 0; token = strtok(0, " \t\n")) {
        Audio_Event_Type_Add_Sample(Event, token);
    }

    float volume = (float)ini.Get_Double(Name(), "Volume", unknown_voc_volume_7f9a70) * 0.01f;
    if (volume > 1.0) {
        volume = 1.0f;
    } else if (volume <= 0) {
        volume = 0.0f;
    }
    Audio_Event_Type_Set_Volume(Event, (int)(volume * 16384.0f));

    Audio_Event_Type_Set_Percent(Event, ini.Get_Int(Name(), "VShift", 0));

    float min_volume = (float)ini.Get_Double(Name(), "MinVolume", unknown_voc_min_volume_7f9a74) * 0.01f;
    if (min_volume > 1.0) {
        min_volume = 1.0f;
    } else if (min_volume <= 0) {
        min_volume = 0.0f;
    }
    Audio_Event_Type_Set_Min_Volume(Event, min_volume);

    ini.Get_String(Name(), "Priority", "NORMAL", buffer, sizeof(buffer));
    int priority = unknown_voc_priority_7f9a6c;
    Audio_Priority_From_Name(buffer, &priority);
    Audio_Event_Type_Set_Priority(Event, priority);

    Audio_Event_Type_Set_Intro(Event, ini.Get_Int(Name(), "Attack", 0));
    Audio_Event_Type_Set_Outro(Event, ini.Get_Int(Name(), "Decay", 0));

    ini.Get_String(Name(), "Control", "", buffer, sizeof(buffer));
    token = strtok(buffer, " \t\n");
    unsigned int control = unknown_voc_control_ad0850;
    if (token != 0) {
        control = 0;
        do {
            Audio_Control_Apply_Keyword(&control, token);
            token = strtok(0, " \t\n");
        } while (token != 0);
    }
    Audio_Event_Type_Set_Flags(Event, control);

    ini.Get_String(Name(), "Type", "", buffer, sizeof(buffer));
    token = strtok(buffer, " \t\n");
    unsigned int type = unknown_voc_type_7f9a78;
    if (token != 0) {
        type = 0x20;
        do {
            Audio_Event_Apply_Keyword(&type, token);
            token = strtok(0, " \t\n");
        } while (token != 0);
    }
    Audio_Event_Type_Set_Type(Event, type);

    Audio_Event_Type_Set_Limit(Event, ini.Get_Int(Name(), "Limit", unknown_voc_limit_7f9a80));
    Audio_Event_Type_Set_Loop(Event, ini.Get_Int(Name(), "Loop", 0));
    Audio_Event_Type_Set_Range(Event, ini.Get_Int(Name(), "Range", unknown_voc_range_7f9a7c));

    ini.Get_String(Name(), "Delay", "", buffer, sizeof(buffer));
    token = strtok(buffer, " \t\n");
    int low = 0;
    if (token != 0) {
        low = atoi(token);
        token = strtok(0, " \t\n");
    }
    int high = low;
    if (token != 0) {
        high = atoi(token);
    }
    Audio_Event_Type_Set_Delay(Event, low, high);

    ini.Get_String(Name(), "FShift", "", buffer, sizeof(buffer));
    token = strtok(buffer, " \t\n");
    low = 0;
    if (token != 0) {
        low = atoi(token);
        token = strtok(0, " \t\n");
    }
    high = low;
    if (token != 0) {
        high = atoi(token);
    }
    Audio_Event_Type_Set_FShift(Event, low, high);
    return true;
}


// AudioEvent *Voc_Play(int voc, float volume, int pan, AudioEventHandle *handle)
// (0x00713DB0): plays a sound effect at a volume (0..1, as 16.16 up to
// 0x4000) and pan; with a handle, reuses its event when it plays the same type
// (else stops it) and keeps the result in the handle (name hypothesis).
#include "shared/audio.hpp"
#include "shared/sound_play.hpp"

AudioEvent *__fastcall Voc_Play(int voc, float volume, int pan, AudioEventHandle *handle)
{
    AudioEventType *type = 0;
    if (voc >= 0 && voc < unknown_VocClass_list_00ad0818.Count()) {
        type = unknown_VocClass_list_00ad0818[voc]->Event;
    }
    AudioEvent *event = 0;
    if (handle != 0) {
        event = Audio_Event_Handle_Event(handle);
        if (event != 0 && Audio_Event_Type_Of(event) != type) {
            Audio_Event_Stop(event);
            event = 0;
        }
    }
    if (type != 0) {
        unsigned int level = (unsigned int)(volume * 16384.0f);
        if (level >= 0x4000) {
            level = 0x4000;
        }
        if (event != 0 || (event = Audio_Event_Create(type)) != 0) {
            Audio_Event_Set_Volume(event, level);
            Audio_Event_Set_Pan(event, pan);
        }
    }
    if (handle != 0) {
        Audio_Event_Handle_Set(handle, event, type);
    }
    return event;
}


// AudioEvent *Voc_Play_At(int voc, CoordStruct const &coord, AudioEventHandle *handle)
// (0x00713E60): plays a sound effect at a coordinate with the volume and pan
// 0x00713F30 gives it there; a silent result stops the handle's event (name
// hypothesis).

AudioEvent *__fastcall Voc_Play_At(int voc, CoordStruct const &coord, AudioEventHandle *handle)
{
    AudioEvent *event = 0;
    AudioEventType *type = 0;
    if (voc >= 0 && voc < unknown_VocClass_list_00ad0818.Count()) {
        type = unknown_VocClass_list_00ad0818[voc]->Event;
    }
    if (handle != 0) {
        event = Audio_Event_Handle_Event(handle);
        if (event != 0 && Audio_Event_Type_Of(event) != type) {
            Audio_Event_Stop(event);
            event = 0;
        }
    }
    if (type != 0) {
        int pan;
        float volume = Voc_Volume_At(coord, &pan, type);
        if (volume > 0) {
            unsigned int level = (unsigned int)(volume * 16384.0f);
            if (level >= 0x4000) {
                level = 0x4000;
            }
            if (event != 0 || (event = Audio_Event_Create(type)) != 0) {
                Audio_Event_Set_Volume(event, level);
                Audio_Event_Set_Pan(event, pan);
            }
        } else if (event != 0) {
            Audio_Event_Stop(event);
            event = 0;
        }
    }
    if (handle != 0) {
        Audio_Event_Handle_Set(handle, event, type);
    }
    return event;
}


// float Voc_Volume_At(CoordStruct const &coord, int *pan, AudioEventType *type)
// (0x00713F30): the volume (0..1) of a sound of a type at a coordinate and its
// pan (0..0x4000), from the coordinate's pixel against the middle of the
// tactical area and the type's range (60 pixels a cell); 0 for a cell the
// type's 0x800 flag refuses and below 0.05 (hypothesis).
#include "shared/tactical.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/tool_tip.hpp"
#include "shared/crt_memory.hpp"
// The cell 0x00713F30 never plays a 0x800-flagged sound in (0x00AD07C0; hypothesis).

float __fastcall Voc_Volume_At(CoordStruct const &coord, int *pan, AudioEventType *type)
{
    *pan = 0;
    float half_width = unknown_rect_839630.Width * 0.5f;
    float half_height = unknown_rect_839630.Height * 0.5f;
    float volume = 0;
    float width = half_width + half_width;
    float range = Audio_Event_Type_Range(type) * 60;
    unsigned int flags = Audio_Event_Type_Flags(type);
    if (flags & 0x800) {
        Cell cell = Cell(coord.X / 256, coord.Y / 256);
        if (cell == unknown_geometry_k) {
            return 0;
        }
        if ((Map[cell].unknown_12c & 0x18) == 0) {
            return 0;
        }
    }
    Point2D pixel;
    unknown_tactical_83996c->Coord_To_Pixel(coord, pixel);
    float dx = pixel.X - half_width;
    float x = abs((int)dx);
    float y = abs((int)(pixel.Y - half_height));
    if (!(flags & 0x40)) {
        x -= half_width;
        y -= half_height;
        if (x < 0) {
            x = 0;
        }
        if (y < 0) {
            y = 0;
        }
    }
    y = y + y;
    if (x < range && y < range && range > 0) {
        float distance = x > y ? x : y;
        volume = (range - distance) / range;
    }
    if (flags & 0x10) {
        if (Audio_Event_Type_Min_Volume(type) > volume) {
            volume = Audio_Event_Type_Min_Volume(type);
        }
    }
    if (volume < 0.05) {
        return 0;
    }
    if (dx < -width) {
        dx = -width;
    } else if (dx > width) {
        dx = width;
    }
    *pan = (int)(dx * 8192.0f / width + 8192.0f);
    return volume;
}


// AudioEvent *Voc_Update_At(CoordStruct const &coord, AudioEventHandle *handle)
// (0x007141B0): sets a handle's event to the volume and pan of its type at a
// coordinate; stops it when silent, and starts a forever-looping type again
// when audible with no event (hypothesis).

AudioEvent *__fastcall Voc_Update_At(CoordStruct const &coord, AudioEventHandle *handle)
{
    AudioEvent *event = 0;
    if (handle != 0) {
        event = Audio_Event_Handle_Event(handle);
        AudioEventType *type = Audio_Event_Handle_Type(handle);
        if (type != 0) {
            int pan;
            float volume = Voc_Volume_At(coord, &pan, type);
            if (volume > 0) {
                if (event == 0 && Audio_Event_Type_Loops_Forever(type)) {
                    event = Audio_Event_Create(type);
                    if (event != 0) {
                        Audio_Event_Set_Flag_8(event);
                    }
                }
                if (event != 0) {
                    unsigned int level = (unsigned int)(volume * 16384.0f);
                    if (level >= 0x4000) {
                        level = 0x4000;
                    }
                    Audio_Event_Set_Volume(event, level);
                    Audio_Event_Set_Pan(event, pan);
                }
            } else if (event != 0) {
                Audio_Event_Stop(event);
                event = 0;
            }
        }
        Audio_Event_Handle_Set(handle, event, type);
    }
    return event;
}


// AudioEvent *Voc_Static_Add(int voc, CoordStruct const &coord, unsigned int flags)
// (0x00714290): a static sound: a node from the pool with a fresh handle, the
// coordinate and flags, the sound played there and the node listed; counts the
// static sounds and their maximum (name hypothesis).
#include "shared/voc_static.hpp"

AudioEvent *__fastcall Voc_Static_Add(int voc, CoordStruct const &coord, unsigned int flags)
{
    if (voc < 0) {
        return 0;
    }
    VocStaticSound *sound = (VocStaticSound *)Audio_Pool_Alloc(unknown_voc_static_pool_ad0844);
    if (sound == 0) {
        return 0;
    }
    sound->Handle.Init();
    Audio_List_Init_Node(sound);
    sound->PauseCount = 0;
    sound->Coord = coord;
    sound->Flags = flags;
    AudioEvent *event = Voc_Play_At(voc, sound->Coord, &sound->Handle);
    Audio_List_Add_Tail(&unknown_voc_static_list_ad07d0, sound);
    Audio_Static_Sound_Count++;
    if (Audio_Static_Sound_Count > Audio_Static_Sound_Maximum) {
        Audio_Static_Sound_Maximum = Audio_Static_Sound_Count;
    }
    return event;
}


// void Voc_Static_Update() (0x00714330): moves every unpaused static sound's
// event to its coordinate (0x007141B0) and drops the sounds whose event ended,
// unless their type loops forever (hypothesis).

void Voc_Static_Update()
{
    VocStaticSound *sound = (VocStaticSound *)Audio_List_First(&unknown_voc_static_list_ad07d0);
    while (sound != 0) {
        VocStaticSound *next = (VocStaticSound *)Audio_List_Next(sound);
        if (sound->PauseCount == 0) {
            Voc_Update_At(sound->Coord, &sound->Handle);
        }
        if (Audio_Event_Handle_Event(&sound->Handle) == 0) {
            AudioEventType *type = Audio_Event_Handle_Type(&sound->Handle);
            if (type == 0 || !Audio_Event_Type_Loops_Forever(type)) {
                Audio_List_Remove(sound);
                Audio_Pool_Free(unknown_voc_static_pool_ad0844, sound);
                Audio_Static_Sound_Count--;
            }
        }
        sound = next;
    }
}


// void Voc_Static_Finish_At(CoordStruct const &coord, unsigned int flags)
// (0x007143B0): lets the static sounds at a coordinate that have any of the
// flags end (hypothesis).

void __fastcall Voc_Static_Finish_At(CoordStruct const &coord, unsigned int flags)
{
    for (VocStaticSound *sound = (VocStaticSound *)Audio_List_First(&unknown_voc_static_list_ad07d0); sound != 0;
         sound = (VocStaticSound *)Audio_List_Next(sound)) {
        if ((sound->Flags & flags) && sound->Coord.X == coord.X && sound->Coord.Y == coord.Y && sound->Coord.Z == coord.Z) {
            Audio_Event_Handle_Finish(&sound->Handle);
        }
    }
}


// void Unknown_00714410() (0x00714410): stops and drops every static sound
// (Do_Restart and Clear_Scenario call it; hypothesis).
#include "shared/vox_class.hpp"

void Unknown_00714410()
{
    VocStaticSound *sound = (VocStaticSound *)Audio_List_First(&unknown_voc_static_list_ad07d0);
    while (sound != 0) {
        VocStaticSound *next = (VocStaticSound *)Audio_List_Next(sound);
        Audio_Event_Handle_Stop(&sound->Handle);
        Audio_List_Remove(sound);
        Audio_Pool_Free(unknown_voc_static_pool_ad0844, sound);
        Audio_Static_Sound_Count--;
        sound = next;
    }
}


// void VocStaticSound::Pause() (0x00714460): one more pause; pauses the
// handle's event (was UnknownOwner_00714460::unknown_bump_and_notify).

void VocStaticSound::Pause()
{
    PauseCount++;
    AudioEvent *event = Audio_Event_Handle_Event(&Handle);
    if (event != 0) {
        Audio_Event_Pause(event);
    }
}


// void VocStaticSound::Resume() (0x00714480): one pause fewer (not below 0);
// resumes the handle's event.

void VocStaticSound::Resume()
{
    if (PauseCount != 0) {
        if (--PauseCount < 0) {
            PauseCount = 0;
        }
        AudioEvent *event = Audio_Event_Handle_Event(&Handle);
        if (event != 0) {
            Audio_Event_Resume(event);
        }
    }
}


// void Voc_Static_Pause() (0x007144B0): pauses every static sound.

void Voc_Static_Pause()
{
    for (VocStaticSound *sound = (VocStaticSound *)Audio_List_First(&unknown_voc_static_list_ad07d0); sound != 0;
         sound = (VocStaticSound *)Audio_List_Next(sound)) {
        sound->Pause();
    }
}


// void Voc_Static_Resume() (0x007144F0): resumes every static sound.

void Voc_Static_Resume()
{
    for (VocStaticSound *sound = (VocStaticSound *)Audio_List_First(&unknown_voc_static_list_ad07d0); sound != 0;
         sound = (VocStaticSound *)Audio_List_Next(sound)) {
        sound->Resume();
    }
}


// void VocClass::Process(CCINIClass &ini) (0x00714540): resets the static
// sounds (their list and a pool of 100), reads the [Defaults] of Sound.ini and
// makes (or finds) a sound effect type for each [SoundList] entry, which then
// reads its own section (name hypothesis).
#include "shared/crt_string.hpp"

void __fastcall VocClass::Process(CCINIClass &ini)
{
    Audio_List_Init(&unknown_voc_static_list_ad07d0);
    unknown_voc_static_pool_ad0844 = Audio_Pool_Create(100, sizeof(VocStaticSound));
    Audio_Static_Sound_Count = 0;
    Audio_Static_Sound_Maximum = 0;

    char buffer[0x800];
    char *token;
    if (ini.Is_Present("Defaults")) {
        unknown_voc_volume_7f9a70 = (float)ini.Get_Double("Defaults", "Volume", unknown_voc_volume_7f9a70);
        unknown_voc_min_volume_7f9a74 = (float)ini.Get_Double("Defaults", "MinVolume", unknown_voc_min_volume_7f9a74);
        ini.Get_String("Defaults", "Priority", "NORMAL", buffer, sizeof(buffer));
        Audio_Priority_From_Name(buffer, &unknown_voc_priority_7f9a6c);

        ini.Get_String("Defaults", "Control", "", buffer, sizeof(buffer));
        token = strtok(buffer, " \t\n");
        if (token != 0) {
            unknown_voc_control_ad0850 = 0;
            do {
                Audio_Control_Apply_Keyword(&unknown_voc_control_ad0850, token);
                token = strtok(0, " \t\n");
            } while (token != 0);
        }

        ini.Get_String("Defaults", "Type", "", buffer, sizeof(buffer));
        token = strtok(buffer, " \t\n");
        if (token != 0) {
            unknown_voc_type_7f9a78 = 0x20;
            do {
                Audio_Event_Apply_Keyword(&unknown_voc_type_7f9a78, token);
                token = strtok(0, " \t\n");
            } while (token != 0);
        }

        unknown_voc_limit_7f9a80 = ini.Get_Int("Defaults", "Limit", unknown_voc_limit_7f9a80);
        unknown_voc_range_7f9a7c = ini.Get_Int("Defaults", "Range", unknown_voc_range_7f9a7c);
    }

    if (ini.Is_Present("SoundList")) {
        int count = ini.Entry_Count("SoundList");
        for (int index = 0; index < count; index++) {
            char name[0x20];
            if (ini.Get_String("SoundList", ini.Get_Entry("SoundList", index), "", name, sizeof(name))) {
                VocClass *voc;
                int voc_index = From_Name(name);
                if (voc_index == -1) {
                    voc = new VocClass(name);
                } else {
                    voc = unknown_VocClass_list_00ad0818[voc_index];
                }
                voc->Read_INI(ini);
            }
        }
    }
}


// void Unknown_00714860() (0x00714860)
// Deletes every sound effect type (each leaves the VocClass list) and, when the static sounds have a pool, stops and
// drops them all and frees the pool (Init_Game and Prog_End call it; hypothesis).
#include "shared/unknown_globals.hpp"

void Unknown_00714860()
{
    while (unknown_VocClass_list_00ad0818.Count() > 0) {
        delete unknown_VocClass_list_00ad0818[0];
    }
    if (unknown_voc_static_pool_ad0844 != 0) {
        Unknown_00714410();
        Audio_Pool_Destroy(unknown_voc_static_pool_ad0844);
        unknown_voc_static_pool_ad0844 = 0;
    }
}


// int VocClass::From_Name(char const *name) (0x00714940): the index of the
// sound effect with this name, -1 if none.

int __fastcall VocClass::From_Name(char const *name)
{
    if (name != 0) {
        for (int index = 0; index < unknown_VocClass_list_00ad0818.Count(); index++) {
            if (_stricmp(name, unknown_VocClass_list_00ad0818[index]->Name()) == 0) {
                return index;
            }
        }
    }
    return -1;
}


// VocClass *VocClass::Find(char const *name) (0x00714990): the sound effect
// with this name; 0 for none, "<none>" or an unknown name (name hypothesis).

VocClass *__fastcall VocClass::Find(char const *name)
{
    if (name != 0 && _stricmp(name, "<none>") != 0) {
        for (int index = 0; index < unknown_VocClass_list_00ad0818.Count(); index++) {
            if (_stricmp(name, unknown_VocClass_list_00ad0818[index]->Name()) == 0) {
                return unknown_VocClass_list_00ad0818[index];
            }
        }
    }
    return 0;
}


// int VocClass::Index_Of(VocClass *voc) (0x00714A30): the index of a VocClass
// in the unit's list, -1 for none.

int __fastcall VocClass::Index_Of(VocClass *voc)
{
    for (int index = 0; index < unknown_VocClass_list_00ad0818.Count(); index++) {
        if (unknown_VocClass_list_00ad0818[index] == voc) {
            return index;
        }
    }
    return -1;
}


#include "shared/com_persist.hpp"
// Save static sound indices and coordinates in list order.
HRESULT __fastcall Voc_Static_Save(IStream *stream)
{
    int count=Audio_List_Count(&unknown_voc_static_list_ad07d0);
    HRESULT result=stream->Write(&count,sizeof(count),0);
    if(SUCCEEDED(result)) {
        VocStaticSound *sound=(VocStaticSound*)Audio_List_First(&unknown_voc_static_list_ad07d0);
        while(sound) {
            struct VocStaticRecord { int Index; CoordStruct Coord; } entry;
            int index=-1;
            entry.Index=index;
            if(sound->Handle.Type) {
                char const *name=Audio_Event_Type_Name(sound->Handle.Type);
                if(name) index=VocClass::From_Name(name);
                entry.Index=index;
            }
            entry.Coord.X=sound->Coord.X;
            entry.Coord.Y=sound->Coord.Y;
            entry.Coord.Z=sound->Coord.Z;
            result=stream->Write(&entry,sizeof(entry),0);
            if(FAILED(result)) return result;
            sound=(VocStaticSound*)Audio_List_Next(sound);
        }
    }
    return result;
}


// 0x00714a60: function not yet identified
//
// Body: 10 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: bool __fastcall FUN_00714a60(int *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00714a60 {

bool __fastcall FUN_00714a60(int *value_pointer)
{
    return *value_pointer != 0;
}

}  // namespace seed_00714a60


// char const *UnknownVocRef_00714A70::unknown_name() const -- 0x00714A70
// Purpose: the name of the referenced sound (0x00405150), "Invalid Voc"
// when there is none.
// Assumptions: the referenced class (+0x00) is unidentified.

struct UnknownVoc_00714a70 {
    char const *unknown_name_405150() const;    // 0x00405150
};
struct UnknownVocRef_00714a70 {
    UnknownVoc_00714a70 *Voc;               // +0x00
    char const *unknown_name() const;
};

char const *UnknownVocRef_00714a70::unknown_name() const
{
    if (Voc != 0) {
        return Voc->unknown_name_405150();
    }
    return "Invalid Voc";
}


// long Voc_Static_Load(IStream *stream) (0x00714B80): drops every static sound,
// stops the audio, then adds back each saved one whose VocClass loops forever
// (name hypothesis).

long __fastcall Voc_Static_Load(IStream *stream)
{
    Unknown_00714410();
    Audio_Stop();
    int count;
    HRESULT result = stream->Read(&count, sizeof(count), 0);
    if (SUCCEEDED(result)) {
        for (int index = 0; index < count; index++) {
            struct VocStaticRecord {
                int Index;
                CoordStruct Coord;
            } entry;
            result = stream->Read(&entry, sizeof(entry), 0);
            if (FAILED(result)) {
                return result;
            }
            if (entry.Index != -1 && entry.Index < unknown_VocClass_list_00ad0818.Count() && entry.Index >= 0) {
                VocClass *voc = unknown_VocClass_list_00ad0818[entry.Index];
                if (voc->Event != 0 && Audio_Event_Type_Loops_Forever(voc->Event)) {
                    CoordStruct coord = entry.Coord;
                    Voc_Static_Add(entry.Index, coord, 0);
                }
            }
        }
    }
    return result;
}


// void Voc_Handle_Set(AudioEventHandle *handle, int voc) (0x00714D10)
// Points the handle at the event type of the sound effect type, when the index is valid (name hypothesis).

void __fastcall Voc_Handle_Set(AudioEventHandle *handle, int voc)
{
    if (voc >= 0 && voc < unknown_VocClass_list_00ad0818.Count()) {
        Audio_Event_Handle_Set(handle, 0, unknown_VocClass_list_00ad0818[voc]->Event);
    }
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<VocClass *>::Clear (0x00714D30; DynamicVectorClass<VocClass *> vtable 0x007ae98c slot 3).
// DynamicVectorClass<VocClass *>::ID_pointer (0x00714D60; DynamicVectorClass<VocClass *> vtable 0x007ae98c slot 5).
// VectorClass<VocClass *>::Fetch (0x00714D80; VectorClass<VocClass *> vtable 0x007ae9ac slot 6).
// VectorClass<VocClass *>::operator equal (0x00714D90; VectorClass<VocClass *> vtable 0x007ae9ac slot 1).
// VectorClass<VocClass *>::Resize (0x00714DD0; VectorClass<VocClass *> vtable 0x007ae9ac slot 2).
// VectorClass<VocClass *>::Clear (0x00714E80; VectorClass<VocClass *> vtable 0x007ae9ac slot 3).
// VectorClass<VocClass *>::ID_object (0x00714EB0; VectorClass<VocClass *> vtable 0x007ae9ac slot 4).
// VectorClass<VocClass *>::ID_pointer (0x00714EF0; VectorClass<VocClass *> vtable 0x007ae9ac slot 5).
// DynamicVectorClass<VocClass *>::Resize (0x00714F10; DynamicVectorClass<VocClass *> vtable 0x007ae98c slot 2).
// DynamicVectorClass<VocClass *>::ID_object (0x00714FC0; DynamicVectorClass<VocClass *> vtable 0x007ae98c slot 4).


extern "C" unsigned char vtable_714ff0[]; extern "C" void __cdecl release_buffer_714ff0(void*); extern "C" void __cdecl release_object_714ff0(void*);
class Leaf_714ff0 { public: void* get(unsigned int flags); };
void* Leaf_714ff0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_714ff0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_714ff0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_714ff0(this); return this; }


extern "C" unsigned char vtable_715040[]; extern "C" void __cdecl release_buffer_715040(void*); extern "C" void __cdecl release_object_715040(void*);
class Leaf_715040 { public: void* get(unsigned int flags); };
void* Leaf_715040::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_715040; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_715040((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_715040(this); return this; }
