// int unknown_check_406f30() -- 0x00406F30
// Purpose: whether the audio library has its main device (the sound system can play).
#include "shared/audio.hpp"
#include "shared/sound_available.hpp"

int unknown_check_406f30()
{
    return Audio_Main_Device != 0;
}
