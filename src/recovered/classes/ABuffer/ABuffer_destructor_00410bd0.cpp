#include "shared/blit.hpp"
ABufferSwapTmp::~ABufferSwapTmp()
{
    delete Surface;
    Surface = 0;
}
