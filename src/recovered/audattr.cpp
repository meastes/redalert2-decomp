// audattr.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/audattr.json carries the addresses and the rows that
// stay in their own files.


#include "shared/audio.hpp"


// void AudioLevelSet::Init() (0x00401000)
// Starts the three levels at full volume (0x4000), 100% frequency and the
// centre pan (0x2000) and sets their ramp rates (frequency 10 per second,
// volume and pan 0x4000 per second), then jumps the volume to full.

void __fastcall AudioLevelSet::Init()
{
    Volume.Init(0x4000);
    Frequency.Init(100);
    Pan.Init(0x2000);
    Frequency.Set_Rate(1000, 10);
    Volume.Set_Rate(1000, 0x4000);
    Pan.Set_Rate(1000, 0x4000);
    Volume.Set(0x4000);
}


// void AudioLevelSet::Update() (0x00401080)
// Advances the volume, frequency and pan levels (each AudioLevel::Update
// 0x004070F0; the last is a tail call).

void AudioLevelSet::Update()
{
    Volume.Update();
    Frequency.Update();
    Pan.Update();
}


// 0x004010a0: function not yet identified
//
// Body: 27 bytes (frozen inventory bound).
// Referenced from 7 sites (unconditional call).
// Ghidra prototype: undefined4 __fastcall AudioLevelSet_Changed_004010a0(byte *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004010a0 {

unsigned int __fastcall AudioLevelSet_Changed_004010a0(unsigned char *level_set)
{
    if ((((*level_set & 2) == 0) && ((level_set[0x28] & 2) == 0)) && ((level_set[0x50] & 2) == 0)) {
        return 0;
    }
    return 1;
}

}  // namespace seed_004010a0


// void __fastcall Audio_Level_Set_Combine(AudioLevelSet *set, AudioLevelSet *other) -- 0x004010C0
// Purpose: combines another level set into this one: volume and frequency
// multiply (16.16 values; 0x4000 and 100 are unity), the pan moves by the
// other's offset from centre, clamped to 0..0x4000.

void __fastcall Audio_Level_Set_Combine(AudioLevelSet *set, AudioLevelSet *other)
{
    set->Volume.Set(((other->Volume.Current >> 16) * (set->Volume.Current >> 16)) >> 14);
    set->Volume.Update();
    set->Frequency.Set((int)((other->Frequency.Current >> 16) * (set->Frequency.Current >> 16)) / 100);
    set->Frequency.Update();
    int pan = (other->Pan.Current >> 16) - 0x2000;
    if (pan != 0) {
        pan += set->Pan.Current >> 16;
        if (pan < 0) {
            pan = 0;
        } else if ((unsigned)pan > 0x4000) {
            pan = 0x4000;
        }
        set->Pan.Set(pan);
    }
    set->Pan.Update();
}


class Leaf_401170 { public: void get(); };
void Leaf_401170::get() { unsigned int level_set_address = (unsigned int)this; unsigned int clear_changed_mask; unsigned int level_flags; level_flags = *(unsigned int*)(level_set_address); clear_changed_mask = 0xfffffffd; level_flags = (level_flags) & (clear_changed_mask); *(unsigned int*)(level_set_address) = level_flags; level_flags = *(unsigned int*)(level_set_address + 0x50); level_flags = (level_flags) & (clear_changed_mask); *(unsigned int*)(level_set_address + 0x50) = level_flags; level_flags = *(unsigned int*)(level_set_address + 0x28); level_flags = (level_flags) & (clear_changed_mask); *(unsigned int*)(level_set_address + 0x28) = level_flags; }


// int AudioLevelSet::Scale_Frequency(int value) (0x00401190): value scaled by the frequency level's
// percent (Frequency.Current, 16.16); the product is unsigned, the division by 100 signed.

int __fastcall AudioLevelSet::Scale_Frequency(int value)
{
    return (int)((Frequency.Current >> 16) * value) / 100;
}
