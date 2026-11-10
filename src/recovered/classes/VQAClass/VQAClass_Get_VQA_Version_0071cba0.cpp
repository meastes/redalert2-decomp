// int VQAClass::Get_VQA_Version() (0x0071CBA0): the handle's first word, or -1.
#include "shared/movie.hpp"

int VQAClass::Get_VQA_Version()
{
    if (Handle != 0) {
        return Handle->unknown_00;
    }
    return -1;
}
