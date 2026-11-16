// audioUnlock_0040a6b0 (0x0040A6B0)
#include "shared/ghidra_types.hpp"

extern "C" __declspec(dllimport) undefined4 __stdcall LeaveCriticalSection(int);  // @ref __imp__LeaveCriticalSection@4=0079a1e0
extern "C" undefined4 Audio_Lock_00831590;

undefined4  audioUnlock_0040a6b0(void)
{
    LeaveCriticalSection((int)((LPCRITICAL_SECTION)&Audio_Lock_00831590));
    return 0;
}

