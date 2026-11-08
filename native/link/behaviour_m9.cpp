// Behaviour-written rows the skirmish harness reaches once technos enter
// their cells (Scout1098, native M9). NOT EXACT; never original code or bytes.
#include "shared/object.hpp"

// 0x00423D90 ObjectClass::unknown_slot47 (leaf src/recovered/classes/ObjectClass/
// 00423d90.cpp, accepted as Leaf_423d90::get: returns 1). CellClass's Occupy_Down
// 0x00473B30 asks it before moving a placed object to the cell's coordinate.
bool ObjectClass::unknown_slot47()
{
    return true;
}

#include "shared/audio.hpp"

// 0x00402030 AudioBankEntry::Get_Source (leaf: returns this + 0x1C, the entry's
// AudioSource member). Natively the member's own offset.
AudioSource *__fastcall AudioBankEntry::Get_Source()
{
    return &Source;
}

// 0x004026A0 Audio_Channel_Lock (seed decompile: a jump through the channel
// driver's +0x28 entry with the channel in ECX): the driver's Lock.
void __fastcall Audio_Channel_Lock(AudioChannel *channel)
{
    channel->Driver->Lock(channel);
}

// 0x004026B0 Audio_Channel_Unlock (seed decompile: the jump through +0x2C):
// the driver's Unlock.
void __fastcall Audio_Channel_Unlock(AudioChannel *channel)
{
    channel->Driver->Unlock(channel);
}

// 0x00401190 AudioLevelSet::Scale_Frequency (seed decompile): value scaled by
// the frequency level's percent (+0x2C, Frequency.Current, 16.16), the
// product unsigned, the division signed.
int __fastcall AudioLevelSet::Scale_Frequency(int value)
{
    return (int)((Frequency.Current >> 16) * (unsigned int)value) / 100;
}
