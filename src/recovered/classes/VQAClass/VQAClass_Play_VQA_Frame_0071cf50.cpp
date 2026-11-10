// void VQAClass::Play_VQA_Frame(int frame) (0x0071CF50): seeks to the frame (-1: the last one) and plays it.
#include "shared/movie.hpp"

void VQAClass::Play_VQA_Frame(int frame)
{
    if (frame == -1) {
        frame = FrameCount - 1;
    }
    if (CurrentFrame != frame) {
        Seek_To_Frame(frame);
        CurrentFrame = frame;
    }
    Play_VQA(frame, 0);
    if ((unsigned)CurrentFrame >= (unsigned)FrameCount) {
        CurrentFrame = 0;
    }
}
