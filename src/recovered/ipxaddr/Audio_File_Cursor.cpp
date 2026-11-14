// 0x00523360: function not yet identified
//
// Body: 64 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall Audio_File_Cursor_00523360(undefined4 *param_1,undefined4 *param_2)
//
// Types and names are Ghidra placeholders (see shared/ghidra_types.hpp);
// rename and retype as the purpose becomes known.
//
// seed-generated: tools/original_target/regenerate_seeds.py may rewrite this file;
// delete this line when editing it by hand.
#include "shared/ghidra_types.hpp"

namespace seed_00523360 {

extern "C" __declspec(dllimport) undefined4 __stdcall EnterCriticalSection(int);  // @ref __imp__EnterCriticalSection@4=0079a1dc
extern "C" __declspec(dllimport) undefined4 __stdcall LeaveCriticalSection(int);  // @ref __imp__LeaveCriticalSection@4=0079a1e0
extern "C" undefined4 audio_file_lock_00a52268;
extern "C" undefined4 audio_file_cursor_x_00a5229c;
extern "C" undefined4 audio_file_cursor_y_00a522a0;
extern "C" undefined4 audio_file_mode_00a522ac;
extern "C" undefined4 audio_file_ready_00a522b0;

void __fastcall Audio_File_Cursor_00523360(undefined4 *x, undefined4 *y)
{
    if ((audio_file_ready_00a522b0 != 0) && (audio_file_mode_00a522ac != 0)) {
        EnterCriticalSection((int)((LPCRITICAL_SECTION)&audio_file_lock_00a52268));
        *x = audio_file_cursor_x_00a5229c;
        *y = audio_file_cursor_y_00a522a0;
        LeaveCriticalSection((int)((LPCRITICAL_SECTION)&audio_file_lock_00a52268));
    }
    return;
}

}  // namespace seed_00523360
