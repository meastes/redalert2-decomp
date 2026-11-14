// mplayer.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/mplayer.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/ms_message_box.hpp"
#include "shared/win32_api.hpp"
#include "shared/ww_message_box.hpp"
#include "shared/string_table.hpp"
#include "shared/lobby.hpp"


// Header-static dynamic initializers of the unit at 0x005A71F0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x005A71F0-0x005A73D1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x005A7210, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_005a71f0.cpp; no source of its own.


// Dynamic initializer (0x005A7240, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_005a71f0.cpp; no source of its own.


// Dynamic initializer (0x005A7260, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_005a71f0.cpp; no source of its own.


// Dynamic initializer (0x005A7280, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_005a71f0.cpp; no source of its own.


// Dynamic initializer (0x005A72A0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_005a71f0.cpp; no source of its own.


// Dynamic initializer (0x005A72C0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_005a71f0.cpp; no source of its own.


// Dynamic initializer (0x005A72E0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_005a71f0.cpp; no source of its own.


// Dynamic initializer (0x005A7320, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_005a71f0.cpp; no source of its own.


// Dynamic initializer (0x005A7350, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_005a71f0.cpp; no source of its own.


// Dynamic initializer (0x005A7380, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_005a71f0.cpp; no source of its own.


// Dynamic initializer (0x005A73B0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_005a71f0.cpp; no source of its own.


// Dynamic initializer (0x005A73C0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_005a71f0.cpp; no source of its own.


// int __stdcall Select_MPlayer_Game_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) (0x005A73E0)
// The common dialog handler first; WM_COMMAND stores the control id in the int that DWL_USER points at
// (TibSun mplayer.cpp's Select_MPlayer_Game_Dialog_Proc without its WM_INITDIALOG part).

int __stdcall Select_MPlayer_Game_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int *retval;

    int rc = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (rc != 0) {
        return rc;
    }

    if (message == 0x111) {
        retval = (int *)GetWindowLongA(window, DWL_USER);
        *retval = (unsigned short)wparam;
    }
    return 0;
}


// int __fastcall OK_Cancel_Message_005A7430(unsigned short const *text) (0x005A7430)
// TibSun mplayer.cpp's Surrender_Dialog: an OK/Cancel message box; 1 when the player picks OK.

int __fastcall OK_Cancel_Message_005A7430(unsigned short const *text)
{
    return WWMessageBox(L" ").Process(text, StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\MPLayer.CPP", 0x6C),
        StringTable::Load_String("TXT_CANCEL", 0, "D:\\Ra2\\MPLayer.CPP", 0x6C), 0, false) == 0;
}


// void __fastcall Free_Pointers_005a7480(DynamicVectorClass<NodeNameType *> *list) (0x005A7480)
// TibSun mplayer.cpp's Clear_Vector: deletes every node of the list, then clears it.

void __fastcall Free_Pointers_005a7480(DynamicVectorClass<NodeNameType *> *list)
{
    int i;

    for (i = 0; i < list->Count(); i++) {
        delete (*list)[i];
    }
    list->Clear();
}
