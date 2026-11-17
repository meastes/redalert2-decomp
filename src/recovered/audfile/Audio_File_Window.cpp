// Audio_File_Window_00522c20 (0x00522C20)

extern "C" int audio_file_window_slots_00a521dc;
extern "C" unsigned int audio_file_ready_00a522b0;
extern "C" unsigned int audio_file_windows_00a522b4;

unsigned int __fastcall Audio_File_Window_00522c20(int index)
{
    if (((audio_file_ready_00a522b0 != 0) && (index >= 0)) && (index < audio_file_window_slots_00a521dc)) {
        return *(unsigned int *)(audio_file_windows_00a522b4 + index * 4);
    }
    return 0;
}

