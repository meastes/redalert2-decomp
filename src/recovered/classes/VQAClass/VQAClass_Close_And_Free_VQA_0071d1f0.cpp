// void VQAClass::Close_And_Free_VQA() (0x0071D1F0): stop.
#include "shared/movie.hpp"
// @ref ?VQA_Unknown_0077BE70@@YIXPAUVQAHandle@@@Z=0077be70

void VQAClass::Close_And_Free_VQA()
{
    if (Opened) {
        VQA_Unknown_0077BE70(Handle);
    }
}
