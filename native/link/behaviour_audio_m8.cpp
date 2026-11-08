// Behaviour-written audio rows the skirmish frames reach once units exist
// (Scout1091, native M8). NOT EXACT; never original code or bytes.
#include "shared/audio.hpp"
#include "shared/voc_class.hpp"

// 0x00406660 (decompile, __fastcall): a looping type (+0x10 bit 0) whose
// loop count (+0x4C) is 0 loops forever.
int __fastcall Audio_Event_Type_Loops_Forever(AudioEventType *type)
{
    return (type->Flags & AUDIO_EVENT_TYPE_LOOPS) != 0 && type->LoopCount == 0 ? 1 : 0;
}

// 0x00402940 Audio_System_Init (instructions; TibSun-era audio library): the
// system's driver and device lists empty (0x004071F0 on +0x00 and +0x0C), no
// busy count (+0x18), the master levels are the system's (+0x20, kept in
// 0x00830B50) and initialised (0x00401000), and the system is up (0x00830B4C).
HRESULT Audio_System_Init()
{
    Audio_List_Init(&Audio_System_Instance->Drivers);
    Audio_List_Init(&Audio_System_Instance->Devices);
    Audio_System_Instance->Busy = 0;
    Audio_Master_Levels = &Audio_System_Instance->Levels;
    Audio_Master_Levels->Init();
    Audio_Initialized = 1;
    return 0;
}

// 0x00403810 AudioDevice::Run_Update: `jmp [ecx+0x10C]`, the device's update
// function (0x00402F60 until the device is run) with the device in ECX.
void AudioDevice::Run_Update()
{
    unknown_update_10c(this);
}

// 0x004010A0 AudioLevelSet::Changed (decompile): any of the three levels has
// its changed bit (Flags bit 1).
int AudioLevelSet::Changed()
{
    return ((Volume.Flags & 2) || (Frequency.Flags & 2) || (Pan.Flags & 2)) ? 1 : 0;
}

// 0x00401170 AudioLevelSet::Clear_Changed (decompile): clears that bit on the
// volume, pan and frequency levels.
void AudioLevelSet::Clear_Changed()
{
    Volume.Flags &= ~2u;
    Pan.Flags &= ~2u;
    Frequency.Flags &= ~2u;
}

extern "C" int __cdecl _stricmp(char const *left, char const *right);   // 0x00781F20
extern "C" void *bsearch(void const *key, void const *base, unsigned long count, unsigned long size,
                         int (*compare)(void const *, void const *));
namespace {
int Compare_Bag_Name(void const *key, void const *entry)
{
    return _stricmp((char const *)key, ((AudioBagEntry const *)entry)->Name);
}
}  // namespace

// 0x004015C0 Audio_Bag_Find (decompile): the CRT's bsearch (0x00782025) of the
// name over the bag's sorted entries (0x24 bytes each) with _stricmp; the
// entry's index, or -1.
int __fastcall Audio_Bag_Find(AudioBag *bag, char const *name)
{
    AudioBagEntry const *found = (AudioBagEntry const *)bsearch(name, bag->Entries, (unsigned)bag->Count,
                                                                 sizeof(AudioBagEntry), Compare_Bag_Name);
    if (found != 0) {
        return (int)(found - bag->Entries);
    }
    return -1;
}

#include "shared/ahandle.hpp"

// 0x0040A6C0 (decompile): the DirectSound object (0x008315AC).
void *unknown_directsound_40a6c0()
{
    return Audio_DirectSound;
}
