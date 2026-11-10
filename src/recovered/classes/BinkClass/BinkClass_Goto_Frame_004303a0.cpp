// void BinkClass::Goto_Frame(int frame) (0x004303A0): BinkGoto(frame, 1).
#include "shared/movie.hpp"

void BinkClass::Goto_Frame(int frame)
{
    BinkGoto(Handle, frame, 1);
}
