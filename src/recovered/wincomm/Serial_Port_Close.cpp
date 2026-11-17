// 0x00734950: function not yet identified
//
// Body: 41 bytes (frozen inventory bound).
// Referenced from 5 sites (unconditional call).
// Ghidra prototype: void __fastcall Serial_Port_Close_00734950(int param_1)
// Callees with accepted source:
//   Debug_Log_004068f0 -> src/recovered/004068f0.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

#include "shared/win32_types.hpp"

namespace seed_00734950 {

extern void  Debug_Log_004068f0(int);
extern "C" __declspec(dllimport) unsigned int __stdcall CloseHandle(int);  // @ref __imp__CloseHandle@4=0079a1d4
extern "C" char s_Closing_serial_port_007fca0c[];

void __fastcall Serial_Port_Close_00734950(int self)
{
    if (*(int *)(self + 0x5c) != -1) {
        Debug_Log_004068f0((int)(s_Closing_serial_port_007fca0c));
        CloseHandle((int)(*(HANDLE *)(self + 0x5c)));
        *(unsigned int *)(self + 0x5c) = 0xffffffff;
    }
    return;
}

}  // namespace seed_00734950
