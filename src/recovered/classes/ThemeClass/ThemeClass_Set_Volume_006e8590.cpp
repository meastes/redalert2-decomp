// void ThemeClass::Set_Volume(int volume) (0x006E8590)
// The theme volume, at most 255.
#include "shared/theme.hpp"

void ThemeClass::Set_Volume(int volume)
{
    if (volume >= 255) {
        volume = 255;
    }
    Volume = volume;
}
