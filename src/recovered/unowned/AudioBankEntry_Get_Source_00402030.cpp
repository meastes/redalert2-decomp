// AudioSource *AudioBankEntry::Get_Source() (0x00402030): the entry's sound (+0x1C).
#include "shared/audio.hpp"

AudioSource *__fastcall AudioBankEntry::Get_Source()
{
    return &Source;
}
