// int __fastcall Audio_File_Init(int count, int size) -- 0x00522560
// Sets the stream file layer up: both locks, a zeroed block of count window slots, the file thread's state (period 100,
// its timing statistics), the file thread at priority 2 ("Unable to change the priority of the Streamer
// Thread" otherwise), the ring with one node of at least 0x2800 bytes and its two queues; clears the recorded
// key states and enters the main window into the window slots (as Add_Modeless_Dialog 0x00522B20 does). Any failure shuts the layer down and returns 0.
// Placeholders: the globals are named by address until the layer is recovered.
#include "shared/audio_file.hpp"
#include "shared/crt_memory.hpp"
#include "shared/debug_printf.hpp"
#include "shared/win32_window.hpp"
// @ref ?unknown_audio_file_lock_a52280@@3UCRITICAL_SECTION@@A=00a52280
// @ref ?unknown_audio_file_lock_a52268@@3UCRITICAL_SECTION@@A=00a52268
// @ref ?Audio_File_Thread@@YGKPAX@Z=005227d0
// @ref ??2@YAPAXI@Z=00782017
// @ref __alldiv=007820c0
// @ref ??_C@_0DL@CKCP@Unable?5to?5change?5the?5priority?5of@=007ccd50
extern "C" short unknown_audio_file_table_00a51fd0[256];

int __fastcall Audio_File_Init(int count, int size)
{
    InitializeCriticalSection(&unknown_audio_file_lock_a52280);
    InitializeCriticalSection(&unknown_audio_file_lock_a52268);
    audio_file_locks_ready_00a522bc = 1;
    if (count <= 0) {
        Audio_File_Shutdown();
        return 0;
    }
    if (size < 0x2800) {
        size = 0x2800;
    }
    audio_file_windows_00a522b4 = (HWND__ **)operator new(count * sizeof(HWND__ *));
    if (audio_file_windows_00a522b4 == 0) {
        Audio_File_Shutdown();
        return 0;
    }
    memset(audio_file_windows_00a522b4, 0, count * sizeof(HWND__ *));
    audio_file_window_count_00a522b8 = 0;
    audio_file_window_slots_00a521dc = count;
    audio_file_thread_stop_00a521e0 = 0;
    audio_file_thread_stopped_00a521e8 = 0;
    unknown_audio_file_00a521f8 = 0;
    unknown_audio_file_period_00a521f0 = 100;
    unknown_audio_file_00a521e4 = 0;
    unknown_audio_file_timer_00a52200.Reset();
    unknown_audio_file_timer_00a52200.Set(unknown_audio_file_period_00a521f0);
    unknown_audio_file_timer_00a52200.Set_08(unknown_audio_file_period_00a521f0 / 4);
    unknown_audio_file_timer_00a52200.Set_20(unknown_audio_file_period_00a521f0 / 4);
    audio_file_thread_00a52260 = CreateThread(0, 0x1000, Audio_File_Thread, (void *)&audio_file_thread_stop_00a521e0, 0, &audio_file_thread_id_00a52264);
    if (audio_file_thread_00a52260 == 0) {
        Audio_File_Shutdown();
        return 0;
    }
    audio_file_thread_running_00a522c0 = 1;
    if (!SetThreadPriority(audio_file_thread_00a52260, 2)) {
        char buffer[1024];
        FormatMessageA(0x1000, 0, GetLastError(), 0, buffer, 1024, 0);
        Debug_Printf("Unable to change the priority of the Streamer Thread - %s\n", buffer);
        Audio_File_Shutdown();
        return 0;
    }
    audio_file_ring_00a522c4 = Audio_Ring_Create();
    if (audio_file_ring_00a522c4 == 0) {
        Audio_File_Shutdown();
        return 0;
    }
    if (!Audio_Ring_Add_Nodes(audio_file_ring_00a522c4, 1, size, 0)) {
        Audio_File_Shutdown();
        return 0;
    }
    Audio_Ring_Reset(audio_file_ring_00a522c4);
    audio_file_read_queue_00a522c8 = Audio_Ring_Acquire(audio_file_ring_00a522c4, 0);
    audio_file_flush_queue_00a522cc = Audio_Ring_Acquire(audio_file_ring_00a522c4, 1);
    if (audio_file_read_queue_00a522c8 == 0 || audio_file_flush_queue_00a522cc == 0) {
        Audio_File_Shutdown();
        return 0;
    }
    audio_file_ready_00a522b0 = 1;
    memset(unknown_audio_file_table_00a51fd0, 0, sizeof(unknown_audio_file_table_00a51fd0));
    if (MainWindow != 0) {
        HWND__ **slot = audio_file_windows_00a522b4;
        HWND__ **free_slot = 0;
        for (int i = 1; i < audio_file_window_slots_00a521dc; i++, slot++) {
            if (*slot == 0) {
                if (free_slot == 0) {
                    free_slot = slot;
                }
            } else if (*slot == MainWindow) {
                return 1;
            }
        }
        if (free_slot != 0) {
            *free_slot = MainWindow;
            audio_file_window_count_00a522b8++;
        }
    }
    return 1;
}
