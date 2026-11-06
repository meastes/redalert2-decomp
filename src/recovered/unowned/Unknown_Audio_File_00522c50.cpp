// Original00522C50: opens the initialized file layer's recording stream.
#include "shared/crt_stdio.hpp"
struct CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION *lock);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION *lock);
extern "C" int DAT_00a522b0;
extern "C" int DAT_00a522ac;
extern "C" _iobuf *DAT_00a522d0;
extern "C" CRITICAL_SECTION DAT_00a52280;
extern "C" char const DAT_007ddc68[];
int __fastcall Unknown_Audio_File_00522c50(char const *name)
{
    if ((DAT_00a522b0 != 0) && (DAT_00a522d0 == 0)) {
        EnterCriticalSection(&DAT_00a52280);
        DAT_00a522d0 = fopen(name, DAT_007ddc68);
        if (DAT_00a522d0 != 0) DAT_00a522ac = 1;
        LeaveCriticalSection(&DAT_00a52280);
        return DAT_00a522d0 != 0;
    }
    return false;
}
