// Audio_File_Cursor_00523360 (0x00523360)
#include "shared/ghidra_types.hpp"

extern "C" __declspec(dllimport) DWORD __stdcall EnterCriticalSection(int);  // @ref __imp__EnterCriticalSection@4=0079a1dc
extern "C" __declspec(dllimport) DWORD __stdcall LeaveCriticalSection(int);  // @ref __imp__LeaveCriticalSection@4=0079a1e0
extern "C" DWORD audio_file_lock_00a52268;
extern "C" DWORD audio_file_cursor_x_00a5229c;
extern "C" DWORD audio_file_cursor_y_00a522a0;
extern "C" DWORD audio_file_mode_00a522ac;
extern "C" DWORD audio_file_ready_00a522b0;

void __fastcall Audio_File_Cursor_00523360(DWORD *x, DWORD *y)
{
    if ((audio_file_ready_00a522b0 != 0) && (audio_file_mode_00a522ac != 0)) {
        EnterCriticalSection((int)((LPCRITICAL_SECTION)&audio_file_lock_00a52268));
        *x = audio_file_cursor_x_00a5229c;
        *y = audio_file_cursor_y_00a522a0;
        LeaveCriticalSection((int)((LPCRITICAL_SECTION)&audio_file_lock_00a52268));
    }
    return;
}

