// void VQMovieHandle::Unknown_Slot7(int frame) (0x005A1910)
// Seeks the VQA player to a frame (0x0071D210, then 0x0071CC60).
#include "shared/movie.hpp"

void VQMovieHandle::Unknown_Slot7(int frame)
{
    Movie->Player->Reset_VQA();
    Movie->Player->Seek_To_Frame(frame);
}
