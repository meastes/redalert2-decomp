// void Audio_File_Close_Stream() -- 0x00522D50
// Flushes the open stream's file into its queue while it is being read (mode 1), then closes it.
// Placeholders: the globals are named by address until the audio file layer is recovered.
#include "shared/audio_file.hpp"

// @ref __imp__EnterCriticalSection@4=0079a1dc
// @ref __imp__LeaveCriticalSection@4=0079a1e0
// @ref ?unknown_audio_file_lock_a52280@@3UCRITICAL_SECTION@@A=00a52280
// @ref ?Audio_Queue_Stdio@@YIHPAUAudioQueue@@PAXHPAH@Z=00408f50
// @ref ?Audio_Queue_Available@@YIHPAUAudioQueue@@@Z=00409240
// @ref ?fflush_0078439c@@YAHPAX@Z=0078439c
extern int fflush_0078439c(void *file);
extern int fclose_0078395b(void *file);

void Audio_File_Close_Stream()
{
    if (audio_file_ready_00a522b0 != 0) {
        EnterCriticalSection(&unknown_audio_file_lock_a52280);
        if (audio_file_ready_00a522b0 != 0) {
            EnterCriticalSection(&unknown_audio_file_lock_a52280);
            if (audio_file_stream_00a522d0 != 0 && audio_file_ring_00a522c4 != 0 && audio_file_mode_00a522ac == 1 && audio_file_flush_queue_00a522cc != 0) {
                Audio_Queue_Stdio(audio_file_flush_queue_00a522cc, audio_file_stream_00a522d0, Audio_Queue_Available(audio_file_flush_queue_00a522cc), 0);
                fflush_0078439c(audio_file_stream_00a522d0);
            }
            LeaveCriticalSection(&unknown_audio_file_lock_a52280);
        }
        audio_file_mode_00a522ac = 0;
        if (audio_file_stream_00a522d0 != 0) {
            fclose_0078395b(audio_file_stream_00a522d0);
            audio_file_stream_00a522d0 = 0;
        }
        LeaveCriticalSection(&unknown_audio_file_lock_a52280);
    }
}
