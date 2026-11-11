// void BinkMovieHandle::Waiting() (0x005A1B00)
// Forwards to the Bink wrapper.
// @ref ?Check_Frame_Ready@BinkClass@@QAEXXZ=004307e0
#include "shared/movie.hpp"

void BinkMovieHandle::Waiting()
{
    Bink->Check_Frame_Ready();
}
