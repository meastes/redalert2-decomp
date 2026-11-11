// void BinkMovieHandle::InitSubtitles(int unknown) (0x005A1B30)
// Forwards to the Bink wrapper.
// @ref ?Set_Subtitle_Callback@BinkClass@@QAEXH@Z=004303e0
#include "shared/movie.hpp"

void BinkMovieHandle::InitSubtitles(int unknown)
{
    Bink->Set_Subtitle_Callback(unknown);
}
