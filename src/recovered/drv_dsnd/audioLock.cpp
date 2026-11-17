// audioLock_0040a6a0 (0x0040A6A0)
#include "shared/ghidra_types.hpp"

extern "C" __declspec(dllimport) DWORD __stdcall EnterCriticalSection(int);
extern "C" DWORD Audio_Lock_00831590;

DWORD  audioLock_0040a6a0(void)
{
    EnterCriticalSection((int)((LPCRITICAL_SECTION)&Audio_Lock_00831590));
    return 0;
}

