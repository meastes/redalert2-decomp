// trim.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/trim.json carries the addresses and the rows that
// stay in their own files.


#include "shared/object.hpp"


// char *__fastcall strtrim(char *buffer) (0x006EEF10)

char *__fastcall strtrim(char *buffer)
{
    if (buffer != 0) {
        char *source = buffer;
        while (*source != 0 && (unsigned char)*source <= ' ') {
            source++;
        }
        if (source != buffer) {
            strcpy(buffer, source);
        }
        for (int index = strlen(buffer) - 1; index >= 0; index--) {
            if (*source != 0 && (unsigned char)buffer[index] <= ' ') {
                buffer[index] = 0;
            } else {
                break;
            }
        }
    }
    return buffer;
}


// wchar_t *__fastcall wcstrim(wchar_t *buffer) (0x006EEF80)

wchar_t *__fastcall wcstrim(wchar_t *buffer)
{
    if (buffer != 0) {
        wchar_t *source = buffer;
        while (*source != 0 && *source <= L' ') {
            source++;
        }
        if (source != buffer) {
            wcscpy(buffer, source);
        }
        for (int index = wcslen(buffer) - 1; index >= 0; index--) {
            if (*source != 0 && buffer[index] <= L' ') {
                buffer[index] = 0;
            } else {
                break;
            }
        }
    }
    return buffer;
}
