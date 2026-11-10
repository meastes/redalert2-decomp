#include "shared/blit.hpp"
ABuffer::~ABuffer()
{
    delete Surface;
    Surface = 0;
}
