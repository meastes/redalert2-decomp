// bool BinkMovieHandle::Unknown_Slot1() (0x005A1AF0)
// Forwards to the Bink wrapper.
// @ref ?Render_Frame@BinkClass@@QAE_NXZ=00430800
#include "shared/movie.hpp"

bool BinkMovieHandle::Unknown_Slot1()
{
    return Bink->Render_Frame();
}
