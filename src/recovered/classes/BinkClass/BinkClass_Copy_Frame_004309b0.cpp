// void BinkClass::Copy_Frame(Surface *surface, int x, int y, bool unknown4) (0x004309B0)
// Copies the current frame into `surface` at x, y; with unknown4 the
// companion's clear area is filled first and the copy uses Bink's 0x80000000 flag.
#include "shared/movie.hpp"

#pragma auto_inline(off)
void BinkClass::Copy_Frame(Surface *surface, int x, int y, bool unknown4)
{
    if (unknown4) {
        Erase_Subtitle(surface);
    }
    void *buffer = surface->Lock();
    if (buffer != 0) {
        int pitch = surface->Stride();
        int height = surface->Get_Height();
        unsigned flags = SurfaceType;
        if (unknown4) {
            flags |= 0x80000000;
        }
        BinkCopyToBuffer(Handle, buffer, pitch, height, x, y, flags);
        surface->Unlock();
    }
    Draw_Subtitle(surface);
}
#pragma auto_inline(on)
