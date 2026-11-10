// bool BinkClass::Is_Finished() (0x00430420): the movie is past its last frame.
#include "shared/movie.hpp"

bool BinkClass::Is_Finished()
{
    return Handle->FrameNum >= Handle->Frames;
}
