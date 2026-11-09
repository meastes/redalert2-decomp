// char const *Name_From_Land(LandType value) -- 0x00482F00
// The INI name of the value, "<none>" out of range.
#include "shared/ccini_class.hpp"
extern char const *LandNames[12];
// @ref ?LandNames@@3PAPBDA=007d3878
extern char const none_name_007cdecc[];
// @ref ?none_name_007cdecc@@3QBDB=007cdecc

char const *__fastcall Name_From_Land(LandType value)
{
    if ((unsigned)value < 12) {
        return LandNames[value];
    }
    return none_name_007cdecc;
}
