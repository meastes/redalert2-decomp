// void BitFontClass::Release_Surface(Surface *surface) (0x00432150)
// Unlocks the surface the font drew on and forgets its buffer.
#include "shared/text_print.hpp"
#include "shared/surface.hpp"

void BitFontClass::Release_Surface(Surface *surface)
{
    surface->Unlock();
    Buffer = 0;
    Pitch = 0;
}
