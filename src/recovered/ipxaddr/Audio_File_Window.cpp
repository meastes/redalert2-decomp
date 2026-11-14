// 0x00522c20: function not yet identified
//
// Body: 33 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall Audio_File_Window_00522c20(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// Adjusted from the Ghidra seed by permute_seeds.py (signedness, comparison order) to
// match the original; edit freely, this file is no longer regenerated.

namespace seed_00522c20 {

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

}  // namespace seed_00522c20
