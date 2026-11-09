// void CDFileClass::Find_Close() (0x00470470)
// Closes the search Find_File opened and marks it idle.
#include "shared/cd_file_class.hpp"
#include "shared/win32_find.hpp"
// @ref ?FindHandle@CDFileClass@@2PAXA=007d27d4

void CDFileClass::Find_Close()
{
    if (FindHandle != INVALID_HANDLE_VALUE) {
        FindClose(FindHandle);
        FindHandle = INVALID_HANDLE_VALUE;
    }
}
