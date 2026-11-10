// Address of the depth value for a screen point: the lock address of the
// backing surface moved by the ring origin, wrapped at the ring end.
#include "shared/blit.hpp"
unsigned short *ABuffer::Get_Buffer_Offset(Point2D point)
{
    char *pointer=(char *)Surface->Lock(point);
    Surface->Unlock();
    char *result=pointer+BufferOffset;
    if(result>=BufferTail) result-=BufferSize;
    return (unsigned short *)result;
}
