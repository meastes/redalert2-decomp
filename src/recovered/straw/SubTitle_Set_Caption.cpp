// void SubTitle::Set_Caption(unsigned short const *string) -- 0x0069B120
// Purpose: replaces the owned text copy (+0x14) with a copy of the string (none for null).
// Assumptions: name from the ra2-decomp YR twin (SubTitle::Set_Caption, straw.cpp).
#include "shared/subtitle.hpp"

extern "C" unsigned int __cdecl wcslen(unsigned short const *string);                                   // 0x00783605
extern "C" unsigned short *__cdecl wcscpy(unsigned short *destination, unsigned short const *source);   // 0x00783689
// @ref _wcslen=00783605
// @ref _wcscpy=00783689
// @ref ??2@YAPAXI@Z=00782017
// @ref ??3@YAXPAX@Z=00781d3b

void SubTitle::Set_Caption(unsigned short const *string)
{
    if (Text != 0) {
        delete [] Text;
        Text = 0;
    }
    if (string != 0) {
        unsigned int length = wcslen(string);
        Text = new unsigned short[length + 1];
        if (Text != 0) {
            wcscpy(Text, string);
        }
    }
}
