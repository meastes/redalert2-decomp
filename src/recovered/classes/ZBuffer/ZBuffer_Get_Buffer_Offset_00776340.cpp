// Point-by-value getter for the ring at original global00839C6C.
// ZBuffer remains the existing project-name hypothesis.
#include "shared/blit.hpp"
unsigned short *ZBuffer::Get_Buffer_Offset(Point2D point)
{
    char *pointer=(char *)Surface->Lock(point);
    Surface->Unlock();
    char *result=pointer+BufferOffset;
    if(result>=BufferTail) result-=BufferSize;
    return (unsigned short *)result;
}
