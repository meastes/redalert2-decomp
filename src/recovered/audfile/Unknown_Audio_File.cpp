// Original00522C50: opens the initialized file layer's recording stream.
#include "shared/crt_stdio.hpp"
struct CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION *lock);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION *lock);
extern "C" int audio_file_ready_00a522b0;
extern "C" int audio_file_mode_00a522ac;
extern "C" _iobuf *audio_file_stream_00a522d0;
extern "C" CRITICAL_SECTION audio_file_lock_00a52280;
extern "C" char const audio_file_open_mode_007ddc68[];
int __fastcall Unknown_Audio_File_00522c50(char const *name)
{
    if ((audio_file_ready_00a522b0 != 0) && (audio_file_stream_00a522d0 == 0)) {
        EnterCriticalSection(&audio_file_lock_00a52280);
        audio_file_stream_00a522d0 = fopen(name, audio_file_open_mode_007ddc68);
        if (audio_file_stream_00a522d0 != 0) audio_file_mode_00a522ac = 1;
        LeaveCriticalSection(&audio_file_lock_00a52280);
        return audio_file_stream_00a522d0 != 0;
    }
    return false;
}
