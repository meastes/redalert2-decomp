// void BinkMovieHandle::Unknown_Slot7(int frame) (0x005A1B40)
// Forwards the seek to the Bink wrapper.
// @ref ?Goto_Frame@BinkClass@@QAEXH@Z=004303a0
#include "shared/movie.hpp"

void BinkMovieHandle::Unknown_Slot7(int frame)
{
    Bink->Goto_Frame(frame);
}
