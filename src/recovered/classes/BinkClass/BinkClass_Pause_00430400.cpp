// void BinkClass::Pause(bool pause) (0x00430400): BinkPause.
#include "shared/movie.hpp"

void BinkClass::Pause(bool pause)
{
    BinkPause(Handle, pause);
}
