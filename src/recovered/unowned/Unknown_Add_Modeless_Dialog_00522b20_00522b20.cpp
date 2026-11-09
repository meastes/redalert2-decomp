// void __fastcall Unknown_Add_Modeless_Dialog_00522b20(HWND dialog) (0x00522B20)
// Files the dialog in the first free slot of the modeless dialog list at
// 0x00A522B4 (the slots before the last one) unless it is listed already, counting it
// (0x00522B80 removes; the mono window 0x0059D900/0x0059DB30 uses both).
#include "shared/audio_file.hpp"
// @ref _DAT_00a521dc=00a521dc
// @ref _DAT_00a522b0=00a522b0
// @ref _DAT_00a522b4=00a522b4
// @ref _DAT_00a522b8=00a522b8

void __fastcall Unknown_Add_Modeless_Dialog_00522b20(HWND__ *dialog)
{
    if (DAT_00a522b0 != 0 && dialog != 0) {
        HWND__ **free_slot = 0;
        HWND__ **slot = DAT_00a522b4;
        for (int index = 1; index < DAT_00a521dc; index++, slot++) {
            if (*slot == 0) {
                if (free_slot == 0) {
                    free_slot = slot;
                }
            } else if (*slot == dialog) {
                return;
            }
        }
        if (free_slot != 0) {
            *free_slot = dialog;
            DAT_00a522b8++;
        }
    }
}
