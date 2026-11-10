// void BinkMovieHandle::Set_Position(int x, int y) (0x005A1B10)
// Forwards to the Bink wrapper.
// @ref ?Set_Position@BinkClass@@QAEXHH@Z=00430280
#include "shared/movie.hpp"

void BinkMovieHandle::Set_Position(int x, int y)
{
    Bink->Set_Position(x, y);
}
