// void BinkMovieHandle::Play() (0x005A1AC0)
// Plays the Bink movie as the current handle.
// @ref ?Play@BinkClass@@QAEXXZ=00430430
#include "shared/movie.hpp"

void BinkMovieHandle::Play()
{
    CurrentMovieHandle = this;
    Bink->Play();
    CurrentMovieHandle = 0;
}
