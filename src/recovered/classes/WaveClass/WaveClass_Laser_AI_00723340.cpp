// void WaveClass::Laser_AI() (0x00723340; expanded into AI, also kept out of line)
// Types 1 and 2 step: fade out.
#include "shared/wave.hpp"

void WaveClass::Laser_AI()
{
    LaserEC -= 6;
    if (LaserEC < 0x20) {
        Delete_Me();
    }
}
