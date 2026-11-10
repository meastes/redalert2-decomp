// VeinholeMonsterClass::Clear_Global_Data (0x00711C70): frees the shared per-cell table.
#include "shared/veinhole.hpp"
// @ref ?unknown_veinhole_buffer_ad07a0@@3PAEA=00ad07a0
// @ref ??3@YAXPAX@Z=00781d3b

void __cdecl VeinholeMonsterClass::Clear_Global_Data()
{
    if (unknown_veinhole_buffer_ad07a0 != 0) {
        delete[] unknown_veinhole_buffer_ad07a0;
        unknown_veinhole_buffer_ad07a0 = 0;
    }
}
