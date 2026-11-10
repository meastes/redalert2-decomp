// Clear a by-value rectangle to FFFF in the ring at original global00839C6C.
#include "shared/blit.hpp"
void ZBuffer::Clear_Region_007761C0(Rect area)
{
    char *locked=(char *)Surface->Lock(Point2D(area.X,area.Y));
    Surface->Unlock();
    char *pointer=locked+BufferOffset;
    if(pointer>=BufferTail) pointer-=BufferSize;
    if(area.Height>0) {
        int rows=area.Height;
        do {
            if(pointer+(unsigned int)area.Width*2u>=BufferTail) {
                unsigned int first=(unsigned int)(BufferTail-pointer)>>1;
                Fill_Words((unsigned short *)pointer,first,0xffff);
                Fill_Words((unsigned short *)BufferHead,area.Width-first,0xffff);
            } else {
                Fill_Words((unsigned short *)pointer,area.Width,0xffff);
            }
            pointer+=(unsigned int)Width*2u;
            if(pointer>=BufferTail) pointer-=BufferSize;
        } while(--rows);
    }
}
