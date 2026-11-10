// void BinkMovieHandle::Pause(bool pause) (0x005A1AB0)
// Forwards to the Bink wrapper.
// @ref ?Pause@BinkClass@@QAEX_N@Z=00430400
#include "shared/movie.hpp"

void BinkMovieHandle::Pause(bool pause)
{
    Bink->Pause(pause);
}
