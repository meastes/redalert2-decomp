// void __fastcall Unknown_007168C0(int side) (0x007168C0)
// Records the side whose speech files EVA plays (0x00AD0958).
#include "shared/vox_speech.hpp"
#include "shared/unknown_globals.hpp"

void __fastcall Unknown_007168C0(int side)
{
    unknown_flag_ad0958 = side;
}
