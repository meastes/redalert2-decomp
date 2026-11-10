// void BinkMovieHandle::Blit() (0x005A1B60)
// Forwards to the Bink wrapper.
// @ref ?Redraw@BinkClass@@QAEXXZ=00430820
#include "shared/movie.hpp"

void BinkMovieHandle::Blit()
{
    Bink->Redraw();
}
