// char const *Name_From_Layer(LayerType value) -- 0x00482FC0
// The INI name of the value, "<none>" out of range.
#include "shared/ccini_class.hpp"
extern char const *LayerNames[5];
// @ref ?LayerNames@@3PAPBDA=007d38c8
extern char const none_name_007cdecc[];
// @ref ?none_name_007cdecc@@3QBDB=007cdecc

char const *__fastcall Name_From_Layer(LayerType value)
{
    if ((unsigned)value < 5) {
        return LayerNames[value];
    }
    return none_name_007cdecc;
}
