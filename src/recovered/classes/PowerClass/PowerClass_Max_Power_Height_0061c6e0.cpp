// int PowerClass::Max_Power_Height() (0x0061C6E0, static)
// One power bar segment per 3 pixels of 0x00ABCD70, rounded up. No direct
// callers (inlined elsewhere).
#include "shared/power.hpp"
// @ref ?unknown_int_abcd70@@3HA=00abcd70

int PowerClass::Max_Power_Height()
{
    return (unknown_int_abcd70 + 3) / 3;
}
