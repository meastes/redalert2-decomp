// int __fastcall Audio_File_Open_Stream(char const *name) -- 0x00522CB0
// Opens the stream's file (mode string 0x007DDC6C), and marks the stream as read when the first queue accepts data.
// Placeholders: the globals are named by address until the audio file layer is recovered.
#include "shared/audio_file.hpp"

// @ref __imp__EnterCriticalSection@4=0079a1dc
// @ref __imp__LeaveCriticalSection@4=0079a1e0
// @ref ?unknown_audio_file_lock_a52280@@3UCRITICAL_SECTION@@A=00a52280
// @ref ?Audio_Queue_Stdio@@YIHPAUAudioQueue@@PAXHPAH@Z=00408f50
// @ref ?Audio_Queue_Available@@YIHPAUAudioQueue@@@Z=00409240
// @ref ?fopen_00783a45@@YAPAXPBD0@Z=00783a45
extern void *fopen_00783a45(char const *name, char const *mode);
extern "C" char const audio_file_open_mode_007ddc6c[];

int __fastcall Audio_File_Open_Stream(char const *name)
{
    if (audio_file_ready_00a522b0 != 0 && audio_file_stream_00a522d0 == 0) {
        EnterCriticalSection(&unknown_audio_file_lock_a52280);
        audio_file_stream_00a522d0 = fopen_00783a45(name, audio_file_open_mode_007ddc6c);
        audio_file_fill_failed_00a522d4 = 0;
        if (audio_file_stream_00a522d0 != 0) {
            audio_file_mode_00a522ac = 2;
            if (Audio_Queue_Stdio(audio_file_read_queue_00a522c8, audio_file_stream_00a522d0, Audio_Queue_Available(audio_file_read_queue_00a522c8), 0) != 1) {
                audio_file_fill_failed_00a522d4 = 1;
            }
        }
        LeaveCriticalSection(&unknown_audio_file_lock_a52280);
        return audio_file_stream_00a522d0 != 0;
    }
    return false;
}
