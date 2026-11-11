// int BinkMovieHandle::Timing() (0x005A1B50)
// Forwards to the Bink wrapper.
// @ref ?Frame_Time@BinkClass@@QAEHXZ=004303c0
#include "shared/movie.hpp"

int BinkMovieHandle::Timing()
{
    return Bink->Frame_Time();
}
