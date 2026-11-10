// void BinkMovieHandle::Unknown_Slot8(int unknown) (0x005A1B30)
// Forwards to the Bink wrapper.
// @ref ?Set_Subtitle_Callback@BinkClass@@QAEXH@Z=004303e0
#include "shared/movie.hpp"

void BinkMovieHandle::Unknown_Slot8(int unknown)
{
    Bink->Set_Subtitle_Callback(unknown);
}
