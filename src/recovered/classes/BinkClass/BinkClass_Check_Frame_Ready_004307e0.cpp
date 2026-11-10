// void BinkClass::Check_Frame_Ready() (0x004307E0): marks the next frame due (BinkWait 0).
#include "shared/movie.hpp"

void BinkClass::Check_Frame_Ready()
{
    if (BinkWait(Handle) == 0) {
        unknown_2d = true;
    }
}
