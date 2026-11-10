// void VQAClass::Handle_Codebook_Event(void *buffer, unsigned length) (0x0071D2C0)
#include "shared/movie.hpp"
// @ref ?VQA_Unknown_0070B7F0@@YIXPAXH@Z=0070b7f0

void VQAClass::Handle_Codebook_Event(void *buffer, unsigned length)
{
    if (unknown_508 != -1) {
        VQA_Unknown_0070B7F0(buffer, length / 2);
    }
    unknown_544 = CurrentFrame;
}
