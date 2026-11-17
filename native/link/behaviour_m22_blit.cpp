// Behaviour row for native M22 (Scout1177). The blitters' backward entries (slot 2),
// behaviour rewrites here before, are exact source now (convert.cpp, lane S1).
#include "shared/blitter.hpp"

// 0x00775980 BlitTrans<unsigned char>::BlitForward (handwritten assembly in the
// original): copies every source byte but colour 0, as blitter.hpp's template does
// for the 16-bit copy.
template <>
void BlitTrans<unsigned char>::BlitForward(void *target, void const *source, int length,
                                           int, void *, void *, int, int) const
{
    for (int index = 0; index < length; index++) {
        unsigned char color = ((unsigned char const *)source)[index];
        if (color != 0) {
            ((unsigned char *)target)[index] = color;
        }
    }
}
