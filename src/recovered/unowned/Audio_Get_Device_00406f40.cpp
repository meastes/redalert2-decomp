// AudioDevice *Audio_Get_Device() -- 0x00406F40
// Purpose: the audio library's main device (0 without one).
#include "shared/audio.hpp"

AudioDevice *Audio_Get_Device()
{
    return Audio_Main_Device;
}
