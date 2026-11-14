// 0x00403810: function not yet identified
//
// Body: 6 bytes (frozen inventory bound).
// Referenced from 2 sites (unconditional call).
// Ghidra prototype: void __fastcall AudioDeviceService_00403810(int param_1)
//
// Types and names are Ghidra placeholders (see shared/ghidra_types.hpp);
// rename and retype as the purpose becomes known.
//
#include "shared/ghidra_types.hpp"

namespace seed_00403810 {

void __fastcall AudioDeviceService_00403810(int device_address)
{
                                        /* WARNING: Could not recover jumptable at 0x00403810. Too many branches */
                                        /* WARNING: Treating indirect jump as call */
    (**(code **)(device_address + 0x10c))();
    return;
}

}  // namespace seed_00403810
