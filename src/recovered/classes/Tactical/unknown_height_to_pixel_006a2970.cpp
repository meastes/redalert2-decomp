// int __fastcall Unknown_006A2970(int z) (0x006A2970): the screen rise of a
// height in leptons, rounded, one pixel more from 728 up (hypothesis). Tactical
// 0x006A27A0 expands it inline.
#include "shared/tactical.hpp"
// @ref __real@8@3ffe8000000000000000=0079a728
// @ref _unknown_scale_abe490=00abe490

int __fastcall Unknown_006A2970(int z)
{
    int bias = 0;
    if (z >= 728) {
        bias = 1;
    }
    return (int)(z * unknown_scale_abe490 + bias + 0.5);
}
