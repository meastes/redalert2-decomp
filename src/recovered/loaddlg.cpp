// loaddlg.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/loaddlg.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/map_seed.hpp"
#include "shared/load_dlg.hpp"
#include "shared/dialog_create.hpp"
#include "shared/ww_message_box.hpp"
#include "shared/cd.hpp"
#include "shared/campaign.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/session_state.hpp"
#include "shared/win_util.hpp"
#include "shared/crt_string.hpp"
#include "shared/crt_memory.hpp"
#include "shared/wol_ladder_list_cell.hpp"
#include "shared/house.hpp"
#include "shared/id_source.hpp"


// Header-static dynamic initializers of the unit at 0x0053AEF0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0053AEF0-0x0053B0D1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0053AF10, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0053aef0.cpp; no source of its own.


// Dynamic initializer (0x0053AF40, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0053aef0.cpp; no source of its own.


// Dynamic initializer (0x0053AF60, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0053aef0.cpp; no source of its own.


// Dynamic initializer (0x0053AF80, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0053aef0.cpp; no source of its own.


// Dynamic initializer (0x0053AFA0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0053aef0.cpp; no source of its own.


// Dynamic initializer (0x0053AFC0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0053aef0.cpp; no source of its own.


// Dynamic initializer (0x0053AFE0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0053aef0.cpp; no source of its own.


// Dynamic initializer (0x0053B020, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0053aef0.cpp; no source of its own.


// Dynamic initializer (0x0053B050, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0053aef0.cpp; no source of its own.


// Dynamic initializer (0x0053B080, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0053aef0.cpp; no source of its own.


// Dynamic initializer (0x0053B0B0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0053aef0.cpp; no source of its own.


// Dynamic initializer (0x0053B0C0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0053aef0.cpp; no source of its own.


// LoadOptionsClass default saved-game file list. Original0053B0E0,75body+5padding.

LoadOptionsClass::LoadOptionsClass()
{
    unknown_04 = 0;
    Description = 0;
    unknown_14 = 0;
    Extension = "SAV";
    unknown_10 = 0x800;
    Files.Clear();
}


// Releases each owned file entry, clears the list and destroys its vector storage.
// Original0053B130,96body/no outerpadding; no direct free of this.

LoadOptionsClass::~LoadOptionsClass()
{
    for (int i = 0; i < Files.Count(); ++i) {
        delete Files[i];
    }
    Files.Clear();
}


// 0x0053b190: function not yet identified
//
// Body: 19 bytes (frozen inventory bound).
// Referenced from 3 sites (unconditional call).
// Ghidra prototype: void __fastcall FUN_0053b190(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0053b190 {

extern unsigned int __fastcall Dialog_0053b770(void);

void __fastcall FUN_0053b190(int object_address)
{
    *(unsigned int *)(object_address + 4) = 1;
    *(unsigned int *)(object_address + 0xc) = 0;
    Dialog_0053b770();
    return;
}

}  // namespace seed_0053b190


// 0x0053b1b0: function not yet identified
//
// Body: 36 bytes (frozen inventory bound).
// Referenced from 2 sites (unconditional call).
// Ghidra prototype: void __thiscall FUN_0053b1b0(int param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0053b1b0 {

extern unsigned int __fastcall Dialog_0053b770(void);
extern "C" unsigned int Scen_00a3d290;

struct Owner_0053b1b0 { void FUN_0053b1b0(int unknown_0x0c); };

void Owner_0053b1b0::FUN_0053b1b0(int unknown_0x0c)
{
    int object_address = (int)this;
    *(unsigned int *)(object_address + 4) = 2;
    if (unknown_0x0c == 0) {
        unknown_0x0c = (int)(Scen_00a3d290 + 0x940);
    }
    *(int *)(object_address + 0xc) = unknown_0x0c;
    Dialog_0053b770();
    return;
}

}  // namespace seed_0053b1b0


// 0x0053b1e0: function not yet identified
//
// Body: 19 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void __fastcall FUN_0053b1e0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0053b1e0 {

extern unsigned int __fastcall Dialog_0053b770(void);

void __fastcall FUN_0053b1e0(int object_address)
{
    *(unsigned int *)(object_address + 4) = 3;
    *(unsigned int *)(object_address + 0xc) = 0;
    Dialog_0053b770();
    return;
}

}  // namespace seed_0053b1e0


// Records how the player left the load dialog (0x0053B200).

void __fastcall LoadOptionsClass::Load_Dialog_On_WM_COMMAND(HWND window, int id, HWND control, int code)
{
    LoadOptionsClass *options = (LoadOptionsClass *)GetWindowLongA(window, 8);
    switch (id) {
    case 0x525:
        if (code == 2 && SendMessageA(control, 0x18b, 0, 0) > 0) {
            options->State = 0x40f;
        }
        break;
    case 0x40f:
        if (code == 0) {
            options->State = id;
        }
        break;
    case 0x686:
        options->State = 2;
        break;
    }
}


// Handles the save dialog's controls: picking a listed game copies its description into the edit field (0x0053B280).

void __fastcall LoadOptionsClass::Save_Dialog_On_WM_COMMAND(HWND window, int id, HWND control, int code)
{
    LoadOptionsClass *options = (LoadOptionsClass *)GetWindowLongA(window, 8);
    switch (id) {
    case 0x527:
        if (code == 1 && SendMessageA(control, 0x18b, 0, 0) > 0) {
            int row = SendMessageA(control, 0x188, 0, 0);
            if (row != -1) {
                FileEntryClass *entry = (FileEntryClass *)SendMessageA(control, 0x199, row, 0);
                if (entry->HasDescription && !entry->NeedsDescription) {
                    HWND edit = GetDlgItem(window, 0x526);
                    SendMessageA(edit, 0x4b2, 0, (LPARAM)entry->Description);
                } else {
                    wchar_t *text = options->Description;
                    if (text != 0) {
                        HWND edit = GetDlgItem(window, 0x526);
                        SendMessageA(edit, 0x4b2, 0, (LPARAM)text);
                    }
                }
                SetFocus(GetDlgItem(window, 0x526));
                SendMessageA(GetDlgItem(window, 0x526), 0xb1, 0, -1);
            }
        }
        break;
    case 0x686:
        if (code == 0) {
            options->State = 2;
        }
        break;
    case 0x6c7:
        if (code == 0) {
            options->State = 1;
        }
        break;
    }
}


// Records how the player left the delete dialog (0x0053B390).

void __fastcall LoadOptionsClass::Delete_Dialog_On_WM_COMMAND(HWND window, int id, HWND control, int code)
{
    LoadOptionsClass *options = (LoadOptionsClass *)GetWindowLongA(window, 8);
    switch (id) {
    case 0x686:
        if (code == 0) {
            options->State = 2;
        }
        break;
    case 0x6c8:
        if (code == 0) {
            options->State = 1;
        }
        break;
    }
}


// The load game dialog's procedure: owner-draw first, then the list columns and the command handler (0x0053B3D0).

LRESULT __stdcall LoadOptionsClass::Load_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result != 0) {
        return result;
    }
    switch (message) {
    case 0x0f:
        unknown_dialog_5e5f20(window);
        return 1;
    case 0x111:
        Load_Dialog_On_WM_COMMAND(window, (unsigned short)wparam, (HWND)lparam, (unsigned short)(wparam >> 16));
        break;
    case 0x216:
        return Unknown_Clamp_Rect_To_Screen_00736D80(window, wparam, (Rect *)lparam);
    case 0x497:
        SendDlgItemMessageA(window, 0x525, 0x4a6, 0xf9, 2);
        SendDlgItemMessageA(window, 0x525, 0x4a6, 0x38, 0xff);
        SendDlgItemMessageA(window, 0x525, 0x4a6, 0, 0x13b);
        break;
    }
    return 0;
}


// The save game dialog's procedure: owner-draw first, then the list columns and the command handler (0x0053B530).

LRESULT __stdcall LoadOptionsClass::Save_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result != 0) {
        return result;
    }
    switch (message) {
    case 0x0f:
        unknown_dialog_5e5f20(window);
        return 1;
    case 0x111:
        Save_Dialog_On_WM_COMMAND(window, (unsigned short)wparam, (HWND)lparam, (unsigned short)(wparam >> 16));
        break;
    case 0x216:
        return Unknown_Clamp_Rect_To_Screen_00736D80(window, wparam, (Rect *)lparam);
    case 0x497:
        SendMessageA(GetDlgItem(window, 0x526), 0xc5, 0x4f, 0);
        SendDlgItemMessageA(window, 0x527, 0x4a6, 0xf9, 2);
        SendDlgItemMessageA(window, 0x527, 0x4a6, 0x38, 0xff);
        SendDlgItemMessageA(window, 0x527, 0x4a6, 0, 0x13b);
        break;
    case 0x4da:
        ((LoadOptionsClass *)GetWindowLongA(window, 8))->State = 1;
        break;
    }
    return 0;
}


// The delete game dialog's procedure: owner-draw first, then the list columns and the command handler (0x0053B650).

LRESULT __stdcall LoadOptionsClass::Delete_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result != 0) {
        return result;
    }
    switch (message) {
    case 0x0f:
        unknown_dialog_5e5f20(window);
        return 1;
    case 0x111:
        Delete_Dialog_On_WM_COMMAND(window, (unsigned short)wparam, (HWND)lparam, (unsigned short)(wparam >> 16));
        break;
    case 0x216:
        return Unknown_Clamp_Rect_To_Screen_00736D80(window, wparam, (Rect *)lparam);
    case 0x497:
        SendDlgItemMessageA(window, 0x528, 0x4a6, 0xf9, 2);
        SendDlgItemMessageA(window, 0x528, 0x4a6, 0x38, 0xff);
        SendDlgItemMessageA(window, 0x528, 0x4a6, 0, 0x13b);
        break;
    }
    return 0;
}


// bool LoadOptionsClass::Dialog() (0x0053B770)
// Runs the load (style 1), save (2) or delete (3) dialog until the player picks a game or leaves.

unsigned int Query_Free_Disk_Kilobytes_00482c80();

bool LoadOptionsClass::Dialog()
{
    HWND dialog = 0;
    HWND list = 0;
    wchar_t buffer[256];
    char filename_buffer[256];

    switch (unknown_04) {
    case 1: {
        dialog = unknown_create_dialog_5ff610(0xb7, (DialogProc)Load_Dialog_Proc, 0);
        list = GetDlgItem(dialog, 0x525);
        HWND title = GetDlgItem(dialog, 0x694);
        SendMessageA(title, 0x4b2, 0, (LPARAM)Load_Menu_Name());
        if (!unknown_session_a3d298.Unknown_00672F10()) {
            HWND button = GetDlgItem(dialog, 0x40c);
            ShowWindow(button, 0);
        }
        break;
    }
    case 2: {
        if (Query_Free_Disk_Kilobytes_00482c80() < (unsigned int)unknown_10) {
            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_DISKFULL", 0, "D:\\Ra2\\LoadDlg.CPP", 0x1ad),
                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\LoadDlg.CPP", 0x1ad), 0, 0, false);
            return false;
        }
        dialog = unknown_create_dialog_5ff610(0x2b4, (DialogProc)Save_Dialog_Proc, 0);
        list = GetDlgItem(dialog, 0x527);
        wchar_t *description = Description;
        HWND edit = GetDlgItem(dialog, 0x526);
        SendMessageA(edit, 0x4b2, 0, (LPARAM)description);
        HWND title = GetDlgItem(dialog, 0x694);
        SendMessageA(title, 0x4b2, 0, (LPARAM)Save_Menu_Name());
        if (!unknown_session_a3d298.Unknown_00672F10()) {
            HWND button = GetDlgItem(dialog, 0x40c);
            ShowWindow(button, 0);
        }
        break;
    }
    case 3: {
        dialog = unknown_create_dialog_5ff610(0x2b5, (DialogProc)Delete_Dialog_Proc, 0);
        list = GetDlgItem(dialog, 0x528);
        HWND title = GetDlgItem(dialog, 0x694);
        SendMessageA(title, 0x4b2, 0, (LPARAM)Delete_Menu_Name());
        if (!unknown_session_a3d298.Unknown_00672F10()) {
            HWND button = GetDlgItem(dialog, 0x40c);
            ShowWindow(button, 0);
        }
        break;
    }
    }

    State = -1;
    if (dialog) {
        SetWindowLongA(dialog, 8, (LONG)this);
        if (list != 0) {
            Fill_List_0053c040(list);
            EnableWindow(GetDlgItem(dialog, 1), bool(SendMessageA(list, 0x18b, 0, 0) > 0));
        }
        unknown_dialog_5ff7c0(dialog);

        do {
            while (State == -1) {
                if (unknown_6000d0() == true) {
                    State = 2;
                }
                if (unknown_14) {
                    ((void (*)())unknown_14)();
                }
                if (!unknown_flag_a40958) {
                    unknown_517370(false);
                }
            }
            if (State == 1 || State == 0x40f) {
                int row = SendMessageA(list, 0x188, 0, 0);
                if (row == -1) {
                    State = -1;
                } else {
                    FileEntryClass *entry = (FileEntryClass *)SendMessageA(list, 0x199, row, 0);
                    switch (unknown_04) {
                    case 1:
                        if (!entry->NeedsDescription) {
                            int old_disk = Unknown_0046E4C0(0x3c);
                            CD::Set_Required_CD(entry->unknown_1a8 == -1 ? -1 : Campaigns[entry->unknown_1a8]->unknown_98);
                            if (!CD().Force_Available(CD::RequiredCD)) {
                                CD::Set_Required_CD(old_disk);
                                State = -1;
                            } else {
                                ShowWindow(dialog, 0);
                                WWMouse->Hide_Mouse();
                                if (!Load(entry->Filename)) {
                                    WWMouse->Show_Mouse();
                                    ShowWindow(dialog, 5);
                                    WWMessageBox(L" ").Process(StringTable::Load_String("TXT_ERROR_LOADING_GAME", 0, "D:\\Ra2\\LoadDlg.CPP", 0x218),
                                        StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\LoadDlg.CPP", 0x218), 0, 0, false);
                                    ShowWindow(dialog, 0);
                                    State = -1;
                                }
                            }
                        }
                        break;
                    case 2: {
                        HWND item = GetDlgItem(dialog, 0x527);
                        HWND disabled = 0;
                        if (item) {
                            disabled = (HWND)SendMessageA(item, 0x4ef, 0, 0);
                            if (disabled) {
                                OwnerDraw::unknown_5e46c0(disabled, true);
                            }
                        }
                        HWND edit = GetDlgItem(dialog, 0x526);
                        SendMessageA(edit, 0x4b3, 0x50, (LPARAM)buffer);
                        wcstrim(buffer);
                        if (wcslen(buffer) == 0) {
                            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_MUSTENTER_DESCRIPTION", 0, "D:\\Ra2\\LoadDlg.CPP", 0x230),
                                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\LoadDlg.CPP", 0x230), 0, 0, false);
                            SetFocus(GetDlgItem(dialog, 0x526));
                            SendMessageA(GetDlgItem(dialog, 0x526), 0xb1, -1, -1);
                            State = -1;
                            break;
                        }
                        char const *filename = 0;
                        if (entry && entry->HasDescription) {
                            filename = entry->Filename;
                        } else {
                            {
                                CCFileClass file;
                                do {
                                    sprintf(filename_buffer, "SAVE%04lX.%3s", rand(), Extension);
                                    file.Set_Name(filename_buffer);
                                } while (file.Is_Available() == true);
                            }
                            filename = filename_buffer;
                        }
                        if (filename) {
                            bool exists = RawFileClass(filename).Is_Available() == true;
                            if (exists && WWMessageBox(L" ").Process(StringTable::Load_String("TXT_CONFIRM_SAVE", 0, "D:\\Ra2\\LoadDlg.CPP", 0x244),
                                    StringTable::Load_String("TXT_YES", 0, "D:\\Ra2\\LoadDlg.CPP", 0x244),
                                    StringTable::Load_String("TXT_NO", 0, "D:\\Ra2\\LoadDlg.CPP", 0x244), 0, false)) {
                                State = -1;
                            } else {
                                ShowWindow(dialog, 0);
                                if (!Save(filename, buffer)) {
                                    ShowWindow(dialog, 5);
                                    WWMessageBox(L" ").Process(StringTable::Load_String("TXT_ERROR_SAVING_GAME", 0, "D:\\Ra2\\LoadDlg.CPP", 0x253),
                                        StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\LoadDlg.CPP", 0x253), 0, 0, false);
                                    State = -1;
                                } else {
                                    ShowWindow(dialog, 5);
                                    OwnerDraw::unknown_5e6330(dialog);
                                    wchar_t const *message = Saved_Message();
                                    WWMessageBox(L" ").Process(message,
                                        StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\LoadDlg.CPP", 0x25b), 0, 0, false);
                                    OwnerDraw::unknown_5e62d0(dialog);
                                    ShowWindow(dialog, 0);
                                    if (Description) {
                                        wcscpy(buffer, Description);
                                    }
                                }
                            }
                        }
                        if (disabled) {
                            OwnerDraw::unknown_5e46c0(disabled, false);
                        }
                        break;
                    }
                    case 3: {
                        swprintf(buffer, L"%s\n\n%s",
                            StringTable::Load_String("TXT_DELETE_FILE_QUERY", 0, "D:\\Ra2\\LoadDlg.CPP", 0x26d), entry->Description);
                        HWND item = GetDlgItem(dialog, 0x528);
                        HWND disabled = 0;
                        if (item) {
                            disabled = (HWND)SendMessageA(item, 0x4ef, 0, 0);
                            if (disabled) {
                                OwnerDraw::unknown_5e46c0(disabled, true);
                            }
                        }
                        if (!WWMessageBox(L" ").Process(buffer,
                                StringTable::Load_String("TXT_YES", 0, "D:\\Ra2\\LoadDlg.CPP", 0x278),
                                StringTable::Load_String("TXT_NO", 0, "D:\\Ra2\\LoadDlg.CPP", 0x278), 0, false)) {
                            Delete_File(entry->Filename);
                            SendMessageA(list, 0x182, row, 0);
                            SendMessageA(list, 0x186, 0, 0);
                            if (SendMessageA(list, 0x18b, 0, 0) > 0) {
                                State = -1;
                            }
                        } else {
                            State = -1;
                        }
                        if (disabled) {
                            OwnerDraw::unknown_5e46c0(disabled, false);
                        }
                        break;
                    }
                    }
                }
            }
        } while (State == -1);
        for (int i = 0; i < Files.Count(); ++i) {
            delete Files[i];
        }
        Files.Clear();
        unknown_dialog_5ff6e0(dialog);
    }
    return State == 0x40f ? true : false;
}


// Picks a save file name that does not exist yet (0x0053BF90).

void LoadOptionsClass::Pick_Filename(char *name)
{
    CCFileClass file;
    do {
        sprintf(name, "SAVE%04lX.%3s", rand(), Extension);
        file.Set_Name(name);
    } while (file.Is_Available() == true);
}


// Deletes every file entry and empties the list (0x0053C010).

void LoadOptionsClass::Clear_List()
{
    for (int i = 0; i < Files.Count(); ++i) {
        delete Files[i];
    }
    Files.Clear();
}


// void LoadOptionsClass::Fill_List_0053c040(HWND list) (0x0053C040)
// Full original 1396+12 body; nominal void/source name qualified by original
// receiver/RET4/caller evidence. No accepted foreign ABI changed.

// Existing canonical rotating-buffer conversion, repeated without broad IPX includes.
wchar_t *__fastcall Narrow_To_Wide_006fa180(char const *text);
// Documented Win32 imports absent from the small shared Win32 subset.
WIN32_IMPORT BOOL WINAPI SystemTimeToFileTime(SYSTEMTIME const *system_time, FILETIME *file_time);
WIN32_IMPORT BOOL WINAPI FileTimeToLocalFileTime(FILETIME const *file_time, FILETIME *local_time);
WIN32_IMPORT int WINAPI GetDateFormatA(DWORD locale, DWORD flags, SYSTEMTIME const *date,
                                      char const *format, char *output, int count);
WIN32_IMPORT int WINAPI GetTimeFormatA(DWORD locale, DWORD flags, SYSTEMTIME const *time,
                                      char const *format, char *output, int count);
extern "C" wchar_t *__cdecl wcscpy(wchar_t *destination, wchar_t const *source);

inline FileEntryClass::FileEntryClass()
    : unknown_120(0), unknown_124(-1), unknown_1a8(-1),
      HasDescription(true), NeedsDescription(false), unknown_1b8(0)
{
    Description[0] = 0;
    Filename[0] = 0;
    *(wchar_t *)unknown_128 = 0;
    LastWriteTime.dwHighDateTime = 0;
    LastWriteTime.dwLowDateTime = 0;
}

void LoadOptionsClass::Fill_List_0053c040(HWND list)
{
    WolLadderListCell cell;
    SYSTEMTIME system_time;
    FILETIME local_time;
    char text[128];
    wchar_t wide[128];
    WIN32_FIND_DATA find;

    for (int i = 0; i < Files.Count(); ++i) {
        delete Files[i];
    }
    Files.Clear();
    if (unknown_04 == 2) {
        FileEntryClass *entry = new FileEntryClass;
        swprintf(entry->Description, StringTable::Load_String("TXT_EMPTY_SLOT", 0, "D:\\Ra2\\LoadDlg.CPP", 0x2f8));
        if (unknown_player_a35db4 != 0) {
            entry->unknown_120 = unknown_id_source_a3d290->TechLevel;
            entry->unknown_124 = unknown_id_source_a3d290->unknown_11fc;
            entry->unknown_1a8 = unknown_id_source_a3d290->unknown_224c;
            wcscpy((wchar_t *)entry->unknown_128, unknown_player_a35db4->Type->UIName);
        } else {
            entry->unknown_120 = 0;
            entry->unknown_124 = unknown_a3d3bc;
            entry->unknown_1a8 = -1;
            swprintf((wchar_t *)entry->unknown_128, L"%s", Narrow_To_Wide_006fa180(unknown_name_a3d3a0));
        }
        GetSystemTime(&system_time);
        SystemTimeToFileTime(&system_time, &entry->LastWriteTime);
        int game_mode = unknown_game_mode_a3d298;
        entry->HasDescription = false;
        entry->unknown_1b8 = game_mode;
        Files.Add(entry);
    }
    sprintf(text, "*.%3s", Extension);
    HANDLE search = FindFirstFileA(text, &find);
    FileEntryClass *entry = 0;
    if (search != INVALID_HANDLE_VALUE) {
        do {
            if ((find.dwFileAttributes & 0x116) == 0) {
                if (entry == 0) {
                    entry = new FileEntryClass;
                }
                if (Read_File_Description(entry, &find) == true) {
                    Files.Add(entry);
                    entry = 0;
                }
            }
        } while (FindNextFileA(search, &find));
        if (entry != 0) {
            delete entry;
        }
        FindClose(search);
    }
    if (Files.Count() > 0) {
        qsort(&Files[0], Files.Count(), sizeof(FileEntryClass *),
              reinterpret_cast<int(__cdecl *)(void const *, void const *)>(Compare_Save_Times));
        SendMessageA(list, 0x184, 0, 0);
        for (int i = 0; i < Files.Count(); ++i) {
            FileEntryClass *file = Files[i];
            if (!file->NeedsDescription) {
                WPARAM row = SendMessageA(list, 0x4cd, 0, (LPARAM)file);
                if (file->unknown_1b8 != 0) {
                    cell.Flags = 1;
                    cell.Text.Set(L"*");
                    SendMessageA(list, 0x4a8, ((DWORD)(WORD)row << 16) | 200, (LPARAM)&cell);
                }
                if (file->LastWriteTime.dwHighDateTime != (DWORD)-1 &&
                    file->LastWriteTime.dwLowDateTime != (DWORD)-1) {
                    FileTimeToLocalFileTime(&file->LastWriteTime, &local_time);
                    FileTimeToSystemTime(&local_time, &system_time);
                    GetDateFormatA(0x400, 1, &system_time, 0, text, 128);
                    cell.Flags = 1;
                    int length = MultiByteToWideChar(0, 0, text, -1, wide, 128);
                    wide[127] = 0;
                    wide[length] = 0;
                    cell.Text.Set(wide);
                    SendMessageA(list, 0x4a8, ((DWORD)(WORD)row << 16) | 0xff, (LPARAM)&cell);
                    GetTimeFormatA(0x400, 0, &system_time, 0, text, 128);
                    cell.Flags = 1;
                    length = MultiByteToWideChar(0, 0, text, -1, wide, 128);
                    wide[127] = 0;
                    wide[length] = 0;
                    cell.Text.Set(wide);
                    SendMessageA(list, 0x4a8, ((DWORD)(WORD)row << 16) | 0x13b, (LPARAM)&cell);
                }
                SendMessageA(list, 0x19a, row, (LPARAM)file);
            }
        }
        switch (unknown_04) {
        case 1: {
            for (int i = 0; i < Files.Count(); ++i) {
                if (Files[i]->HasDescription && !Files[i]->NeedsDescription) {
                    SendMessageA(list, 0x186, i, 0);
                    SendMessageA(list, 0x197, i, 0);
                    break;
                }
            }
            break;
        }
        case 2:
        case 3:
            SendMessageA(list, 0x186, 0, 0);
            SendMessageA(list, 0x197, 0, 0);
            break;
        default:
            break;
        }
    }
}


// bool LoadOptionsClass::Has_Saved_Games_0053c5c0() (0x0053C5C0)
// Whether a file with the options' extension other than SAVEGAME.NET exists (and is not a hidden, system or
// directory entry, attribute mask 0x116).

bool LoadOptionsClass::Has_Saved_Games_0053c5c0()
{
    char pattern[64];
    WIN32_FIND_DATA find;
    bool found = false;
    sprintf(pattern, "*.%3s", Extension);
    HANDLE handle = FindFirstFileA(pattern, &find);
    if (handle != INVALID_HANDLE_VALUE) {
        do {
            if (!(find.dwFileAttributes & 0x116) && _stricmp(find.cFileName, "SAVEGAME.NET") != 0) {
                found = true;
                break;
            }
        } while (FindNextFileA(handle, &find));
    }
    FindClose(handle);
    return found;
}


// int __cdecl Compare_Save_Times(FileEntryClass **first, FileEntryClass **second) (0x0053C650)
// The save list's qsort callback: the entry written last sorts first.

int __cdecl Compare_Save_Times(FileEntryClass **first, FileEntryClass **second)
{
    FileEntryClass *left = *first;
    FileEntryClass *right = *second;
    return -CompareFileTime(&left->LastWriteTime, &right->LastWriteTime);
}


// bool __stdcall Load_Game_With_Dialog(char const *filename) (0x0053C680)
// Shows the "loading" dialog (text TXT_LOADING) with the mouse hidden, loads the game, then restores
// the mouse hide count and closes the dialog.

bool __stdcall Load_Game_With_Dialog(char const *filename)
{
    int mouse_state = WWMouse->Get_Mouse_State();
    HWND dialog = unknown_cancel_dialog_6001e0(StringTable::Load_String("TXT_LOADING", 0, "D:\\Ra2\\LoadDlg.CPP", 0x41C), 0, 0);
    WWMouse->Hide_Mouse();
    if (dialog) {
        unknown_dialog_5ff7c0(dialog);
    }
    unknown_flag_a40d04 = false;
    unknown_flag_a40380 = false;
    unknown_session_a3d298.Unknown_00672E70();
    bool loaded = Unknown_00656890(filename);
    while (WWMouse->Get_Mouse_State() < mouse_state) {
        WWMouse->Show_Mouse();
    }
    while (WWMouse->Get_Mouse_State() > mouse_state) {
        WWMouse->Hide_Mouse();
    }
    if (dialog) {
        unknown_dialog_5ff6e0(dialog);
    }
    return loaded;
}


// bool __stdcall Save_Game_With_Dialog(char const *filename, wchar_t const *description) (0x0053C760)
// Shows the "saving game" dialog (TXT_SAVING_GAME) while the game is written, then closes it.

bool __stdcall Save_Game_With_Dialog(char const *filename, wchar_t const *description)
{
    HWND dialog = unknown_cancel_dialog_6001e0(StringTable::Load_String("TXT_SAVING_GAME", 0, "D:\\Ra2\\LoadDlg.CPP", 0x453), 0, 0);
    WWMouse->Hide_Mouse();
    if (dialog) {
        unknown_dialog_5ff7c0(dialog);
    }
    bool saved = Unknown_006554A0(filename, description, 0);
    WWMouse->Show_Mouse();
    if (dialog) {
        unknown_dialog_5ff6e0(dialog);
    }
    return saved;
}


// bool __stdcall Delete_Save_File(char const *filename) (0x0053C7D0)

bool __stdcall Delete_Save_File(char const *filename)
{
    bool deleted = DeleteFileA(filename) == 1;
    return deleted;
}


// bool LoadOptionsClass::Read_File_Description(FileEntryClass *entry, WIN32_FIND_DATA const *find) (0x0053C7F0)
// The base class's slot 4: reads the saved game summary of a *.SAV file (not SAVEGAME.NET) into the list entry:
// description, player name, game type, file name and write time. A game whose first summary value differs from
// 0x007F1B18 is flagged NeedsDescription.

extern "C" wchar_t *__cdecl wcscpy(wchar_t *dest, wchar_t const *source);
extern "C" unsigned int __cdecl strlen(char const *text);
#pragma intrinsic(strlen, strcpy)

bool LoadOptionsClass::Read_File_Description(FileEntryClass *entry, WIN32_FIND_DATA const *find)
{
    if (entry || find) {
        char const *filename = find->cFileName;
        if (_stricmp(filename, "SAVEGAME.NET") != 0) {
            SaveVersionInfo info;
            bool valid = Get_Savefile_Info(filename, info);
            if (valid) {
                if (info.Get_Internal_Version() != unknown_savegame_version_7f1b18) {
                    wcscpy(entry->Description, info.Get_Scenario_Description());
                    entry->NeedsDescription = true;
                } else {
                    wcscpy(entry->Description, info.Get_Scenario_Description());
                    entry->NeedsDescription = false;
                }
                entry->HasDescription = valid;
                entry->unknown_120 = info.Get_Scenario_Number();
                entry->unknown_1a8 = info.Get_Campaign_Number();
                entry->unknown_1b8 = info.Get_Game_Type();
                strcpy(entry->Filename, filename);
                wcscpy((wchar_t *)entry->unknown_128, info.Get_Player_House());
                if (strlen(entry->Filename) == 0) {
                    strcpy(entry->Filename, find->cAlternateFileName);
                }
                entry->LastWriteTime.dwHighDateTime = find->ftLastWriteTime.dwHighDateTime;
                entry->LastWriteTime.dwLowDateTime = find->ftLastWriteTime.dwLowDateTime;
                return true;
            }
        }
    }
    return false;
}


extern "C" unsigned char receiver_53c970[];
extern "C" unsigned char argument_53c970[];
extern "C" unsigned int __fastcall request_53c970(unsigned int ecx_word,unsigned int edx_word,unsigned int stackword0,unsigned int stackword1);
extern "C" void __cdecl recover_53c970() { request_53c970((unsigned int)receiver_53c970, 0, (unsigned int)argument_53c970, 0x4c0u); }


extern "C" unsigned char receiver_53c990[];
extern "C" unsigned char argument_53c990[];
extern "C" unsigned int __fastcall request_53c990(unsigned int ecx_word,unsigned int edx_word,unsigned int stackword0,unsigned int stackword1);
extern "C" void __cdecl recover_53c990() { request_53c990((unsigned int)receiver_53c990, 0, (unsigned int)argument_53c990, 0x4c6u); }


extern "C" unsigned char receiver_53c9b0[];
extern "C" unsigned char argument_53c9b0[];
extern "C" unsigned int __fastcall request_53c9b0(unsigned int ecx_word,unsigned int edx_word,unsigned int stackword0,unsigned int stackword1);
extern "C" void __cdecl recover_53c9b0() { request_53c9b0((unsigned int)receiver_53c9b0, 0, (unsigned int)argument_53c9b0, 0x4ccu); }


extern "C" unsigned char receiver_53c9d0[];
extern "C" unsigned char argument_53c9d0[];
extern "C" unsigned int __fastcall request_53c9d0(unsigned int ecx_word,unsigned int edx_word,unsigned int stackword0,unsigned int stackword1);
extern "C" void __cdecl recover_53c9d0() { request_53c9d0((unsigned int)receiver_53c9d0, 0, (unsigned int)argument_53c9d0, 0x4d2u); }


// LoadOptionsClass::`scalar deleting destructor'(unsigned int) (0x0053C9F0)
// Compiler-generated with the vtable; the destructor 0x0053B130 and the VectorClass destructor are expanded inline.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<FileEntryClass *>::Clear (0x0053CA70; DynamicVectorClass<FileEntryClass *> vtable 0x007a5844 slot 3).
// DynamicVectorClass<FileEntryClass *>::ID_pointer (0x0053CAA0; DynamicVectorClass<FileEntryClass *> vtable 0x007a5844 slot 5).
// VectorClass<FileEntryClass *>::Fetch (0x0053CAC0; VectorClass<FileEntryClass *> vtable 0x007a5864 slot 6).
// VectorClass<FileEntryClass *>::operator equal (0x0053CAD0; VectorClass<FileEntryClass *> vtable 0x007a5864 slot 1).
// VectorClass<FileEntryClass *>::Resize (0x0053CB10; VectorClass<FileEntryClass *> vtable 0x007a5864 slot 2).
// VectorClass<FileEntryClass *>::Clear (0x0053CBC0; VectorClass<FileEntryClass *> vtable 0x007a5864 slot 3).
// VectorClass<FileEntryClass *>::ID_object (0x0053CBF0; VectorClass<FileEntryClass *> vtable 0x007a5864 slot 4).
// VectorClass<FileEntryClass *>::ID_pointer (0x0053CC30; VectorClass<FileEntryClass *> vtable 0x007a5864 slot 5).
// DynamicVectorClass<FileEntryClass *>::Resize (0x0053CC50; DynamicVectorClass<FileEntryClass *> vtable 0x007a5844 slot 2).
// DynamicVectorClass<FileEntryClass *>::ID_object (0x0053CD00; DynamicVectorClass<FileEntryClass *> vtable 0x007a5844 slot 4).


extern "C" unsigned char vtable_53cd30[]; extern "C" void __cdecl release_buffer_53cd30(void*); extern "C" void __cdecl release_object_53cd30(void*);
class Leaf_53cd30 { public: void* get(unsigned int flags); };
void* Leaf_53cd30::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_53cd30; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_53cd30((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_53cd30(this); return this; }


extern "C" unsigned char vtable_53cd80[]; extern "C" void __cdecl release_buffer_53cd80(void*); extern "C" void __cdecl release_object_53cd80(void*);
class Leaf_53cd80 { public: void* get(unsigned int flags); };
void* Leaf_53cd80::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_53cd80; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_53cd80((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_53cd80(this); return this; }
