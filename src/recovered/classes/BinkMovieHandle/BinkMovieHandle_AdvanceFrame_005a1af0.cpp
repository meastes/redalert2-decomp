// bool BinkMovieHandle::AdvanceFrame() (0x005A1AF0)
// Forwards to the Bink wrapper.
// @ref ?Render_Frame@BinkClass@@QAE_NXZ=00430800
#include "shared/movie.hpp"

bool BinkMovieHandle::AdvanceFrame()
{
    return Bink->Render_Frame();
}
