// void WaveClass::unknown_func_723340() (0x00723340; expanded into AI, also kept out of line)
// Types 1 and 2 step: fade out.
#include "shared/wave.hpp"

void WaveClass::unknown_func_723340()
{
    LaserEC -= 6;
    if (LaserEC < 0x20) {
        unknown_slot60();
    }
}
