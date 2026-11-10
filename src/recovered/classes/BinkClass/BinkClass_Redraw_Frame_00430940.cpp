// void BinkClass::Redraw_Frame(Surface *surface, int x, int y) (0x00430940)
// Copies the current frame into `surface` at x, y again (with the companion
// areas cleared first and drawn after).
#include "shared/movie.hpp"
// @ref ?Draw_Subtitle@BinkClass@@QAEXPAVSurface@@@Z=00430a30
// @ref ?Erase_Subtitle@BinkClass@@QAEXPAVSurface@@@Z=00430af0

void BinkClass::Redraw_Frame(Surface *surface, int x, int y)
{
    Erase_Subtitle(surface);
    void *buffer = surface->Lock();
    if (buffer != 0) {
        int pitch = surface->Stride();
        int height = surface->Get_Height();
        BinkCopyToBuffer(Handle, buffer, pitch, height, x, y, SurfaceType | 0x80000000);
        surface->Unlock();
    }
    Draw_Subtitle(surface);
}
