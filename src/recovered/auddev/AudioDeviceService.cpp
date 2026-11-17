// AudioDeviceService_00403810 (0x00403810)
#include "shared/ghidra_types.hpp"

void __fastcall AudioDeviceService_00403810(int device_address)
{
                                        /* WARNING: Could not recover jumptable at 0x00403810. Too many branches */
                                        /* WARNING: Treating indirect jump as call */
    (**(code **)(device_address + 0x10c))();
    return;
}

