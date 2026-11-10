// VeinholeMonsterClass::unknown_func_711c90 (0x00711C90; Clear_Growth as TiberiumClass names it): drops this monster's growth queue and tables.
#include "shared/veinhole.hpp"
// @ref ??3@YAXPAX@Z=00781d3b

void VeinholeMonsterClass::unknown_func_711c90()
{
    if (unknown_9c != 0) {
        unknown_9c->Clear();
        delete unknown_9c;
        unknown_9c = 0;
    }
    if (unknown_a0 != 0) {
        delete[] unknown_a0;
        unknown_a0 = 0;
    }
    if (unknown_b0 != 0) {
        delete[] unknown_b0;
        unknown_b0 = 0;
    }
    unknown_98 = 0;
}
