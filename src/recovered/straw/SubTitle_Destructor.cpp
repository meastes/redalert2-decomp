// SubTitle::~SubTitle() -- 0x0069B0F0
// Purpose: frees the owned text copy (+0x14).
// Assumptions: name from the ra2-decomp YR twin (SubTitle::~SubTitle, straw.cpp).
#include "shared/subtitle.hpp"

SubTitle::~SubTitle()
{
    if (Text != 0) {
        delete [] Text;
    }
}
