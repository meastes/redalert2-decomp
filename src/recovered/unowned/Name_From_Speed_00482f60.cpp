// char const *Name_From_Speed(SpeedType value) -- 0x00482F60
// The INI name of the value, "<none>" out of range.
#include "shared/ccini_class.hpp"
extern char const *SpeedNames[8];
// @ref ?SpeedNames@@3PAPBDA=007d38a8
extern char const none_name_007cdecc[];
// @ref ?none_name_007cdecc@@3QBDB=007cdecc

char const *__fastcall Name_From_Speed(SpeedType value)
{
    if ((unsigned)value < 8) {
        return SpeedNames[value];
    }
    return none_name_007cdecc;
}
