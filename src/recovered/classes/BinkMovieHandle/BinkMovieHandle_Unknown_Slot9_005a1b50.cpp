// int BinkMovieHandle::Unknown_Slot9() (0x005A1B50)
// Forwards to the Bink wrapper.
// @ref ?Frame_Time@BinkClass@@QAEHXZ=004303c0
#include "shared/movie.hpp"

int BinkMovieHandle::Unknown_Slot9()
{
    return Bink->Frame_Time();
}
