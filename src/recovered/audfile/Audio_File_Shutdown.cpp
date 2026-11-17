// void __fastcall Audio_File_Shutdown() -- 0x00522930
// Flushes the stream file into its queue while it is being read, closes it (Audio_File_Close_Stream's body),
// frees the block 0x00A522B4, stops the file thread (waits up to five seconds), releases the two queues and the
// ring, and deletes the locks. Placeholders: the globals are named by address until the layer is recovered.
#include "shared/audio_file.hpp"

// @ref __imp__EnterCriticalSection@4=0079a1dc
// @ref __imp__LeaveCriticalSection@4=0079a1e0
// @ref __imp__DeleteCriticalSection@4=0079a1d8
// @ref __imp__Sleep@4=0079a1e4
// @ref __imp__WaitForSingleObject@8=0079a1d0
// @ref __imp__CloseHandle@4=0079a1d4
// @ref ?unknown_audio_file_lock_a52280@@3UCRITICAL_SECTION@@A=00a52280
// @ref ?unknown_audio_file_lock_a52268@@3UCRITICAL_SECTION@@A=00a52268
// @ref ?Audio_Queue_Stdio@@YIHPAUAudioQueue@@PAXHPAH@Z=00408f50
// @ref ?Audio_Queue_Available@@YIHPAUAudioQueue@@@Z=00409240
// @ref ?fflush_0078439c@@YAHPAX@Z=0078439c
// @ref ?fclose_0078395b@@YAHPAX@Z=0078395b
// @ref ??3@YAXPAX@Z=00781d3b
extern int fflush_0078439c(void *file);
extern int fclose_0078395b(void *file);

void __fastcall Audio_File_Shutdown()
{
    if (audio_file_ready_00a522b0 != 0) {
        EnterCriticalSection(&unknown_audio_file_lock_a52280);
        if (audio_file_stream_00a522d0 != 0 && audio_file_ring_00a522c4 != 0 && audio_file_mode_00a522ac == 1 && audio_file_flush_queue_00a522cc != 0) {
            Audio_Queue_Stdio(audio_file_flush_queue_00a522cc, audio_file_stream_00a522d0, Audio_Queue_Available(audio_file_flush_queue_00a522cc), 0);
            fflush_0078439c(audio_file_stream_00a522d0);
        }
        LeaveCriticalSection(&unknown_audio_file_lock_a52280);
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
    if (audio_file_windows_00a522b4 != 0) {
        operator delete(audio_file_windows_00a522b4);
        audio_file_windows_00a522b4 = 0;
    }
    audio_file_window_count_00a522b8 = 0;
    if (audio_file_thread_running_00a522c0 != 0) {
        audio_file_thread_stop_00a521e0 = 1;
        while (audio_file_thread_stopped_00a521e8 == 0) {
            Sleep(0);
        }
        WaitForSingleObject(audio_file_thread_00a52260, 5000);
        CloseHandle(audio_file_thread_00a52260);
        audio_file_thread_running_00a522c0 = 0;
    }
    if (audio_file_read_queue_00a522c8 != 0) {
        Audio_Queue_Release(audio_file_read_queue_00a522c8);
        audio_file_read_queue_00a522c8 = 0;
    }
    if (audio_file_flush_queue_00a522cc != 0) {
        Audio_Queue_Release(audio_file_flush_queue_00a522cc);
        audio_file_flush_queue_00a522cc = 0;
    }
    if (audio_file_ring_00a522c4 != 0) {
        Audio_Ring_Destroy(audio_file_ring_00a522c4);
        audio_file_ring_00a522c4 = 0;
    }
    if (audio_file_locks_ready_00a522bc != 0) {
        DeleteCriticalSection(&unknown_audio_file_lock_a52280);
        DeleteCriticalSection(&unknown_audio_file_lock_a52268);
        audio_file_locks_ready_00a522bc = 0;
    }
    audio_file_ready_00a522b0 = 0;
}
