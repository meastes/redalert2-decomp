// phoneed.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/phoneed.json carries the addresses and the rows that
// stay in their own files.


#define VECTOR_IMPLEMENT_ALL
#include "shared/phone_edit.hpp"
#include "shared/dialog_create.hpp"
#include "shared/ms_message_box.hpp"
#include "shared/session_state.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/string_table.hpp"
#include "shared/vector_class.hpp"
#include "shared/crt_string.hpp"
#include "shared/wol_ladder_list_cell.hpp"
#include "shared/win32_window.hpp"
#include "shared/serial_ed.hpp"
#include "shared/debug_printf.hpp"
#define MAKELONG(a, b) ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define MAKEWPARAM(l, h) ((WPARAM)(DWORD)MAKELONG(l, h))


// PhoneEd.cpp (shared/phone_edit.hpp; TibSun phoneed.cpp).

extern "C" wchar_t *__cdecl wcscpy(wchar_t *destination, wchar_t const *source);   // 0x00783689
extern "C" wchar_t *__cdecl wcsncpy(wchar_t *destination, wchar_t const *source, unsigned int count);   // 0x00783622
extern "C" unsigned int __cdecl wcslen(wchar_t const *string);   // 0x00783605
extern "C" int __cdecl wcscmp(wchar_t const *left, wchar_t const *right);   // 0x007837D3
extern "C" int __cdecl swprintf(wchar_t *buffer, wchar_t const *format, ...);   // 0x00783764

extern "C" wchar_t *__cdecl _wcsupr(wchar_t *string);   // 0x00796EF6
char *__fastcall strtrim(char *string);   // 0x006EEF10
wchar_t *__fastcall wcstrim(wchar_t *buffer);   // 0x006EEF80


static int __cdecl Phone_Compare(void const *p1, void const *p2);

// 0x0060D970
bool PhoneEdit::Dialog()
{
    _Dialog = unknown_create_dialog_5ff610(0xEE, Phone_Dialog_Proc, 0);
    if (_Dialog) {
        Changed = false;
        RC = -1;
        SetWindowLongA(_Dialog, DWL_USER, (LONG)this);
        Build_Phone_Listbox();
        unknown_dialog_5ff7c0(_Dialog);
        while (RC < 0) {
            if (unknown_6000d0() == true) {
                break;
            }
            unknown_517370(false);
        }
        unknown_dialog_5ff6e0(_Dialog);
        if (Changed == true) {
            unknown_session_a3d298.Write_MultiPlayer_Settings();
        }
    }
    if (RC != 1) {
        return false;
    }
    return true;
}


// 0x0060D9F0
BOOL __stdcall Phone_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    HWND handle;
    int rc = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (rc == 0) {
        PhoneEdit *_this = (PhoneEdit *)GetWindowLongA(window, DWL_USER);
        switch (message) {
            case 0x497:
                SendDlgItemMessageA(window, 0x5E5, 0xC5, 20, 0);
                SendDlgItemMessageA(window, 0x5E0, 0x4A6, 144, 2);
                SendDlgItemMessageA(window, 0x5E0, 0x4A6, 121, 150);
                SendDlgItemMessageA(window, 0x5E0, 0x4A6, 0, 275);
                break;
            case 0x111:
                switch ((unsigned short)wparam) {
                    case 0x5E4:
                        if ((unsigned short)(wparam >> 16) == 0) {
                            _this->Edit();
                        }
                        break;
                    case 0x661:
                        if ((unsigned short)(wparam >> 16) == 0) {
                            _this->Add();
                        }
                        break;
                    case 0x5E0:
                        if ((unsigned short)(wparam >> 16) == 1) {
                            handle = GetDlgItem(_this->_Dialog, 0x5E0);
                            if (handle != 0) {
                                if (SendMessageA(handle, 0x18B, 0, 0) > 0) {
                                    unknown_session_a3d298.CurPhoneIdx = SendMessageA(handle, 0x188, 0, 0);
                                    if (unknown_session_a3d298.CurPhoneIdx > -1) {
                                        handle = GetDlgItem(_this->_Dialog, 0x5E5);
                                        SendMessageA(handle, 0x4B4, 0,
                                            (LPARAM)unknown_session_a3d298.PhoneBook[unknown_session_a3d298.CurPhoneIdx]->Number);
                                        SetFocus(handle);
                                    }
                                }
                            }
                        }
                        break;
                    case 1:
                        if ((unsigned short)(wparam >> 16) == 0) {
                            if (_this->Dial() == true) {
                                _this->RC = 1;
                            }
                        }
                        break;
                    case 2:
                        if ((unsigned short)(wparam >> 16) == 0) {
                            _this->RC = 2;
                        }
                        break;
                    case 0x53B:
                        if ((unsigned short)(wparam >> 16) == 0) {
                            _this->Delete();
                        }
                        break;
                }
                break;
        }
        rc = 0;
    }
    return rc;
}


// 0x0060DC80: dials the typed number; a number not in the book gets a new entry with default settings.
bool PhoneEdit::Dial()
{
    HWND handle = GetDlgItem(_Dialog, 0x5E5);
    SendMessageA(handle, 0x4B5, sizeof(Number), (LPARAM)Number);
    HWND handle2 = GetDlgItem(_Dialog, 0x5E0);
    unknown_session_a3d298.CurPhoneIdx = SendMessageA(handle2, 0x188, 0, 0);
    if (unknown_session_a3d298.CurPhoneIdx == -1
        || strcmp(unknown_session_a3d298.PhoneBook[unknown_session_a3d298.CurPhoneIdx]->Number, Number)) {
        if (strlen(Number) == 0) {
            return false;
        }
        PhoneEntryClass *p_entry = new PhoneEntryClass();
        if (p_entry == 0) {
            return false;
        }
        wcscpy(p_entry->Name, StringTable::Load_String("TXT_NONAME", 0, "D:\\Ra2\\PhoneEd.cpp", 0xE7));
        strcpy(p_entry->Number, Number);
        p_entry->Settings.Port = 0;
        p_entry->Settings.IRQ = -1;
        p_entry->Settings.Baud = -1;
        p_entry->Settings.DialMethod = 0;
        p_entry->Settings.InitStringIndex = -1;
        p_entry->Settings.CallWaitStringIndex = 3;
        p_entry->Settings.CallWaitString[0] = 0;
        unknown_session_a3d298.PhoneBook.Add(p_entry);
        Build_Phone_Listbox();
        Changed = true;
        unknown_session_a3d298.CurPhoneIdx = unknown_session_a3d298.PhoneBook.ID(p_entry);
    }
    return true;
}


// 0x0060DE40
void PhoneEdit::Build_Phone_Listbox()
{
    HWND handle;
    int i;
    wchar_t item[80];
    char number[80];
    WolLadderListCell thecell;

    handle = GetDlgItem(_Dialog, 0x5E0);
    if (handle) {
        SendMessageA(handle, 0x184, 0, 0);
        if (unknown_session_a3d298.PhoneBook.Count() > 1) {
            qsort((void *)(&unknown_session_a3d298.PhoneBook[0]), unknown_session_a3d298.PhoneBook.Count(),
                  sizeof(PhoneEntryClass *), Phone_Compare);
        }
        for (i = 0; i < unknown_session_a3d298.PhoneBook.Count(); i++) {
            if (!(wcslen(unknown_session_a3d298.PhoneBook[i]->Name))) {
                wcscpy(item, L" ");
            } else {
                wcsncpy(item, unknown_session_a3d298.PhoneBook[i]->Name, 20);
                item[20] = 0;
            }
            int row = SendMessageA(handle, 0x4CD, 0, (LPARAM)item);
            if (!strlen(unknown_session_a3d298.PhoneBook[i]->Number)) {
                wcscpy(item, L" ");
            } else {
                strncpy(number, unknown_session_a3d298.PhoneBook[i]->Number, 20);
                number[20] = 0;
                swprintf(item, L"%hs", number);
            }
            thecell.Flags = 1;
            thecell.Text.Set(item);
            SendMessageA(handle, 0x4A8, MAKEWPARAM(150, row), (LPARAM)&thecell);
            if (unknown_session_a3d298.PhoneBook[i]->Settings.Baud != -1) {
                swprintf(item, L"%d", unknown_session_a3d298.PhoneBook[i]->Settings.Baud);
            } else {
                swprintf(item, L"[%s]", StringTable::Load_String("TXT_DEFAULT", 0, "D:\\Ra2\\PhoneEd.cpp", 0x141));
            }
            thecell.Flags = 1;
            thecell.Text.Set(item);
            SendMessageA(handle, 0x4A8, MAKEWPARAM(275, row), (LPARAM)&thecell);
        }
        if (SendMessageA(handle, 0x18B, 0, 0) == 0 || unknown_session_a3d298.CurPhoneIdx < -1) {
            unknown_session_a3d298.CurPhoneIdx = -1;
        } else {
            if (unknown_session_a3d298.CurPhoneIdx >= SendMessageA(handle, 0x18B, 0, 0)) {
                unknown_session_a3d298.CurPhoneIdx = 0;
            }
        }
        if (unknown_session_a3d298.CurPhoneIdx > -1) {
            SendMessageA(handle, 0x186, unknown_session_a3d298.CurPhoneIdx, 0);
            handle = GetDlgItem(_Dialog, 0x5E5);
            if (handle != 0) {
                SendMessageA(handle, 0x4B4, 0, (LPARAM)unknown_session_a3d298.PhoneBook[unknown_session_a3d298.CurPhoneIdx]->Number);
            }
        }
    }
}


// 0x0060E0E0
void PhoneEdit::Select()
{
    HWND handle = GetDlgItem(_Dialog, 0x5E0);
    if (handle != 0) {
        if (SendMessageA(handle, 0x18B, 0, 0) > 0) {
            unknown_session_a3d298.CurPhoneIdx = SendMessageA(handle, 0x188, 0, 0);
            if (unknown_session_a3d298.CurPhoneIdx > -1) {
                handle = GetDlgItem(_Dialog, 0x5E5);
                SendMessageA(handle, 0x4B4, 0, (LPARAM)unknown_session_a3d298.PhoneBook[unknown_session_a3d298.CurPhoneIdx]->Number);
                SetFocus(handle);
            }
        }
    }
}


// 0x0060E160
void PhoneEdit::Add()
{
    PhoneEntryClass *p_entry = new PhoneEntryClass();
    if (p_entry == 0) {
        return;
    }
    p_entry->Name[0] = 0;
    p_entry->Number[0] = 0;
    p_entry->Settings.Port = 0;
    p_entry->Settings.IRQ = -1;
    p_entry->Settings.Baud = -1;
    p_entry->Settings.DialMethod = 0;
    p_entry->Settings.InitStringIndex = -1;
    p_entry->Settings.CallWaitStringIndex = 3;
    p_entry->Settings.CallWaitString[0] = 0;
    ShowWindow(_Dialog, SW_HIDE);
    UpdateWindow(MainWindow);
    if (PhoneEntryEdit().Dialog(p_entry) == true) {
        unknown_session_a3d298.PhoneBook.Add(p_entry);
        Changed = true;
        Build_Phone_Listbox();
        unknown_session_a3d298.CurPhoneIdx = unknown_session_a3d298.PhoneBook.ID(p_entry);
        if (unknown_session_a3d298.CurPhoneIdx > -1) {
            HWND handle = GetDlgItem(_Dialog, 0x5E0);
            if (handle != 0) {
                SendMessageA(handle, 0x186, unknown_session_a3d298.CurPhoneIdx, 0);
            }
            handle = GetDlgItem(_Dialog, 0x5E5);
            if (handle != 0) {
                SendMessageA(handle, 0x4B4, 0, (LPARAM)unknown_session_a3d298.PhoneBook[unknown_session_a3d298.CurPhoneIdx]->Number);
            }
        }
    } else {
        delete p_entry;
    }
    ShowWindow(_Dialog, SW_SHOW);
}


// 0x0060E2E0
void PhoneEdit::Edit()
{
    HWND handle = GetDlgItem(_Dialog, 0x5E0);
    if (handle != 0) {
        unknown_session_a3d298.CurPhoneIdx = SendMessageA(handle, 0x188, 0, 0);
    }
    if (unknown_session_a3d298.CurPhoneIdx <= -1) {
        return;
    }
    PhoneEntryClass *p_entry = new PhoneEntryClass();
    if (p_entry == 0) {
        return;
    }
    PhoneEntryClass *p_oldentry = unknown_session_a3d298.PhoneBook[unknown_session_a3d298.CurPhoneIdx];
    (*p_entry) = (*p_oldentry);
    ShowWindow(_Dialog, SW_HIDE);
    UpdateWindow(MainWindow);
    if (PhoneEntryEdit().Dialog(p_entry) == true) {
        Changed = true;
        (*p_oldentry) = (*p_entry);
        Build_Phone_Listbox();
        unknown_session_a3d298.CurPhoneIdx = unknown_session_a3d298.PhoneBook.ID(p_oldentry);
        if (unknown_session_a3d298.CurPhoneIdx > -1) {
            SendMessageA(handle, 0x186, unknown_session_a3d298.CurPhoneIdx, 0);
            handle = GetDlgItem(_Dialog, 0x5E5);
            if (handle != 0) {
                SendMessageA(handle, 0x4B4, 0, (LPARAM)p_oldentry->Number);
            }
        }
    }
    ShowWindow(_Dialog, SW_SHOW);
    delete p_entry;
}


// 0x0060E420 (also expanded in Phone_Dialog_Proc)
void PhoneEdit::Delete()
{
    HWND handle = GetDlgItem(_Dialog, 0x5E0);
    if (handle == 0) {
        return;
    }
    unknown_session_a3d298.CurPhoneIdx = SendMessageA(handle, 0x188, 0, 0);
    if (unknown_session_a3d298.CurPhoneIdx <= -1) {
        return;
    }
    unknown_session_a3d298.PhoneBook.Delete(unknown_session_a3d298.CurPhoneIdx);
    Changed = true;
    Build_Phone_Listbox();
    if (unknown_session_a3d298.CurPhoneIdx == -1) {
        handle = GetDlgItem(_Dialog, 0x5E5);
        if (handle != 0) {
            SendMessageA(handle, 0x4B4, 0, (LPARAM)"");
        }
    }
}


// 0x0060E4C0
bool PhoneEntryEdit::Dialog(PhoneEntryClass *phone)
{
    if (phone == 0) {
        return false;
    }
    _Dialog = unknown_create_dialog_5ff610(0xEF, Edit_Phone_Dialog_Proc, 0);
    if (_Dialog) {
        Phone = phone;
        RC = -1;
        SetWindowLongA(_Dialog, DWL_USER, (LONG)this);
        Init();
        unknown_dialog_5ff7c0(_Dialog);
        while (RC < 0) {
            if (unknown_6000d0() == true) {
                break;
            }
            unknown_517370(false);
        }
        unknown_dialog_5ff6e0(_Dialog);
    }
    if (RC != 1) {
        return false;
    }
    return true;
}


// 0x0060E540
BOOL __stdcall Edit_Phone_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    wchar_t buffer[64];
    int rc;

    rc = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (rc == 0) {
        PhoneEntryEdit *_this = (PhoneEntryEdit *)GetWindowLongA(window, DWL_USER);
        switch (message) {
            case 0x497:
                SendDlgItemMessageA(window, 0x5E5, 0xC5, 20, 0);
                SendDlgItemMessageA(window, 0x5DF, 0xC5, 20, 0);
                break;
            case 0x111:
                switch ((unsigned short)wparam) {
                    case 0x663:
                        if ((unsigned short)(wparam >> 16) != 0) {
                            SendMessageA((HWND)lparam, 0xF1, TRUE, 0);
                            SendDlgItemMessageA(window, 0x664, 0xF1, 0, 0);
                        }
                        break;
                    case 0x664:
                        if ((unsigned short)(wparam >> 16) != 0) {
                            SendMessageA((HWND)lparam, 0xF1, TRUE, 0);
                            SendDlgItemMessageA(window, 0x663, 0xF1, 0, 0);
                            UpdateWindow(window);
                            ShowWindow(window, SW_HIDE);
                            UpdateWindow(MainWindow);
                            SerialEdDialog().Dialog_0066ca70(&_this->TempSettings);
                            ShowWindow(window, SW_SHOW);
                        }
                        break;
                    case 0x5E5:
                        if ((unsigned short)(wparam >> 16) == 0x501 || (unsigned short)(wparam >> 16) == 0x200) {
                            SendMessageA((HWND)lparam, 0x4B3, 64, (LPARAM)buffer);
                            wcstrim(buffer);
                            SendMessageA((HWND)lparam, 0x4B2, 0, (LPARAM)buffer);
                        }
                        break;
                    case 1:
                        if ((unsigned short)(wparam >> 16) == 0) {
                            _this->Save();
                            _this->RC = 1;
                        }
                        break;
                    case 2:
                        if ((unsigned short)(wparam >> 16) == 0) {
                            _this->RC = 2;
                        }
                        break;
                    case 0x5DF:
                        if ((unsigned short)(wparam >> 16) == 0x501 || (unsigned short)(wparam >> 16) == 0x200) {
                            SendMessageA((HWND)lparam, 0x4B5, 64, (LPARAM)buffer);
                            strtrim((char *)buffer);
                            SendMessageA((HWND)lparam, 0x4B4, 0, (LPARAM)buffer);
                        }
                        break;
                }
                break;
        }
        rc = 0;
    }
    return rc;
}


// 0x0060E7A0
void PhoneEntryEdit::Save()
{
    bool custom;

    HWND handle = GetDlgItem(_Dialog, 0x5E5);
    if (handle != 0) {
        SendMessageA(handle, 0x4B3, sizeof(Phone->Name) / sizeof(Phone->Name[0]), (LPARAM)Phone->Name);
        if (wcslen(Phone->Name) > 0) {
            _wcsupr(Phone->Name);
        } else {
            wcscpy(Phone->Name, StringTable::Load_String("TXT_NONAME", 0, "D:\\Ra2\\PhoneEd.cpp", 0x2F5));
        }
    }
    Debug_Printf("Name: %ls\n", Phone->Name);
    handle = GetDlgItem(_Dialog, 0x5DF);
    if (handle != 0) {
        SendMessageA(handle, 0x4B5, 21, (LPARAM)Phone->Number);
    }
    Debug_Printf("Number: %s\n", Phone->Number);
    handle = GetDlgItem(_Dialog, 0x664);
    if (handle) {
        custom = SendMessageA(handle, 0xF0, 0, 0) == 1;
    } else {
        custom = false;
    }
    if (custom == true) {
        Phone->Settings = TempSettings;
    } else {
        Phone->Settings.Port = 0;
        Phone->Settings.IRQ = -1;
        Phone->Settings.Baud = -1;
        Phone->Settings.DialMethod = 0;
        Phone->Settings.InitStringIndex = -1;
        Phone->Settings.CallWaitStringIndex = 3;
        Phone->Settings.CallWaitString[0] = 0;
    }
    Debug_Printf("Settings: Port %d, Baud %d\n", Phone->Settings.Port, Phone->Settings.Baud);
}


// 0x0060E900
void PhoneEntryEdit::Init()
{
    bool custom;

    HWND handle = GetDlgItem(_Dialog, 0x5E5);
    SendMessageA(handle, 0x4B2, 0, (LPARAM)Phone->Name);
    handle = GetDlgItem(_Dialog, 0x5DF);
    SendMessageA(handle, 0x4B4, 0, (LPARAM)Phone->Number);
    if (Phone->Settings.Port == 0 || Phone->Settings.IRQ == -1 || Phone->Settings.Baud == -1) {
        TempSettings = unknown_session_a3d298.SerialDefaults;
        custom = false;
    } else {
        TempSettings = Phone->Settings;
        custom = true;
    }
    handle = GetDlgItem(_Dialog, 0x664);
    if (handle != 0) {
        SendMessageA(handle, 0xF1, custom == true, 0);
    }
    handle = GetDlgItem(_Dialog, 0x663);
    if (handle != 0) {
        SendMessageA(handle, 0xF1, custom == false, 0);
    }
}


// 0x0060E9F0
static int __cdecl Phone_Compare(void const *p1, void const *p2)
{
    PhoneEntryClass *pe1, *pe2;
    int result;

    pe1 = *((PhoneEntryClass **)p1);
    pe2 = *((PhoneEntryClass **)p2);
    result = wcscmp(pe1->Name, pe2->Name);
    if (!result) {
        result = strcmp(pe1->Number, pe2->Number);
    }
    return result;
}
