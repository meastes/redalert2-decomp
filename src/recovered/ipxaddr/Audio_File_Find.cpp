// int __fastcall Audio_File_Find(int id) -- 0x00522BD0
// The index of id among the 0x00A521DC entries of the id block 0x00A522B4, remembering the last hit
// (0x00A522D8/0x00A522DC); -1 when absent or the layer is not set up.
#include "shared/audio_file.hpp"

int __fastcall Audio_File_Find(int id)
{
    if (audio_file_ready_00a522b0 != 0) {
        HWND__ **slots = audio_file_windows_00a522b4;
        if (id == audio_file_found_id_00a522d8) {
            return audio_file_found_index_00a522dc;
        }
        for (int i = 0; i < audio_file_window_slots_00a521dc; i++) {
            if ((int)slots[i] == id) {
                audio_file_found_id_00a522d8 = id;
                audio_file_found_index_00a522dc = i;
                return i;
            }
        }
    }
    return -1;
}
