// serialed.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/serialed.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/serial_ed.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/session_state.hpp"
#include "shared/modem_guest.hpp"
#include "shared/string_table.hpp"
#include "shared/crt_string.hpp"
#include "shared/ms_message_box.hpp"
#include "shared/null_modem.hpp"
#include "shared/ww_message_box.hpp"
#include "shared/serial_progress.hpp"
#include "shared/debug_printf.hpp"


// Header-static dynamic initializers of the unit at 0x0066C880: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0066C880-0x0066CA61); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0066C8A0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0066c880.cpp; no source of its own.


// Dynamic initializer (0x0066C8D0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0066c880.cpp; no source of its own.


// Dynamic initializer (0x0066C8F0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0066c880.cpp; no source of its own.


// Dynamic initializer (0x0066C910, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0066c880.cpp; no source of its own.


// Dynamic initializer (0x0066C930, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0066c880.cpp; no source of its own.


// Dynamic initializer (0x0066C950, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0066c880.cpp; no source of its own.


// Dynamic initializer (0x0066C970, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0066c880.cpp; no source of its own.


// Dynamic initializer (0x0066C9B0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0066c880.cpp; no source of its own.


// Dynamic initializer (0x0066C9E0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0066c880.cpp; no source of its own.


// Dynamic initializer (0x0066CA10, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0066c880.cpp; no source of its own.


// Dynamic initializer (0x0066CA40, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0066c880.cpp; no source of its own.


// Dynamic initializer (0x0066CA50, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0066c880.cpp; no source of its own.


// SerialEd.cpp
extern "C" char *__cdecl _strupr(char *string);
extern "C" char *__cdecl strchr(char const *string, int character);
char *__fastcall strtrim(char *string);
extern unsigned short *unknown_baud_names_7f3070[6];
extern "C" int __cdecl swscanf(unsigned short const *text, unsigned short const *format, ...);
extern char const *unknown_port_names_7f305c[4];
extern "C" unsigned short *__cdecl wcscpy(unsigned short *destination, unsigned short const *source);
extern "C" int __cdecl swprintf(unsigned short *buffer, unsigned short const *format, ...);
extern "C" char *__cdecl strncpy(char *destination, char const *source, unsigned int count);

bool SerialEdDialog::Dialog_0066ca70(SerialSettingsType *settings)
{
    Result = -1;
    Window = unknown_create_dialog_5ff610(0xED, Dialog_Proc_0066cb00, 0);
    if (Window) {
        SetWindowLongA(Window, DWL_USER, (LONG)this);
        Init_0066d370(settings);
        unknown_dialog_5ff7c0(Window);
        while (Result < 0) {
            if (unknown_6000d0() == true) {
                break;
            }
            unknown_517370(false);
        }
        unknown_dialog_5ff6e0(Window);
        if (Result == 1) {
            *settings = Settings;
        }
    }
    if (Result == 1) {
        return true;
    }
    return false;
}


int __stdcall SerialEdDialog::Dialog_Proc_0066cb00(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result) {
        return result;
    }
    SerialEdDialog *dialog = (SerialEdDialog *)GetWindowLongA(window, DWL_USER);
    switch (message) {
        case 0x497: {
            HWND control = GetDlgItem(window, 0x65D);
            if (control) {
                SendMessageA(control, 0xC5, 40, 0);
            }
            control = GetDlgItem(window, 0x65F);
            if (control) {
                SendMessageA(control, 0xC5, 62, 0);
            }
            control = GetDlgItem(window, 0x65A);
            if (control) {
                SendMessageA(control, 0xC5, 15, 0);
            }
            break;
        }
        case 0x111:
            switch ((unsigned short)wparam) {
                case 1:
                    if ((unsigned short)(wparam >> 16) == 0 && dialog->Read_Settings_0066cf60() == true) {
                        dialog->Result = 1;
                    }
                    break;
                case 2:
                    if ((unsigned short)(wparam >> 16) == 0) {
                        dialog->Result = 2;
                    }
                    break;
                case 0x3EA:
                    if ((unsigned short)(wparam >> 16)) {
                        SendMessageA((HWND)lparam, 0xF1, 1, 0);
                    }
                    break;
                case 0x3EB:
                    if ((unsigned short)(wparam >> 16)) {
                        SendMessageA((HWND)lparam, 0xF1, 1, 0);
                    }
                    break;
                case 0x657:
                    if ((unsigned short)(wparam >> 16) == 1) {
                        int row = SendMessageA((HWND)lparam, CB_GETCURSEL, 0, 0);
                        if (SendMessageA((HWND)lparam, CB_GETITEMDATA, row, 0) == 4) {
                            SendMessageA((HWND)lparam, 0x4BD, row, (LPARAM)dialog->Buffer);
                            dialog->Show_Text_0066dae0(0x65F);
                        } else {
                            SetFocus(0);
                        }
                    }
                    break;
                case 0x659:
                    if ((unsigned short)(wparam >> 16) == 1) {
                        int row = SendMessageA((HWND)lparam, CB_GETCURSEL, 0, 0);
                        if (SendMessageA((HWND)lparam, CB_GETITEMDATA, row, 0) == 3) {
                            SendMessageA((HWND)lparam, 0x4BD, row, (LPARAM)dialog->Buffer);
                            dialog->Show_Text_0066dae0(0x65A);
                        } else {
                            SetFocus(0);
                        }
                    }
                    break;
                case 0x65A:
                    if ((unsigned short)(wparam >> 16) == 0x501 || (unsigned short)(wparam >> 16) == 0x200) {
                        dialog->Read_Custom_Call_Wait_0066dcf0();
                    }
                    break;
                case 0x65B:
                    if ((unsigned short)(wparam >> 16)) {
                        SendMessageA((HWND)lparam, 0xF1, 1, 0);
                        SendDlgItemMessageA(window, 0x65C, 0xF1, 0, 0);
                    }
                    break;
                case 0x65F:
                    if ((unsigned short)(wparam >> 16) == 0x501 || (unsigned short)(wparam >> 16) == 0x200) {
                        dialog->Read_Custom_Port_0066db50();
                    }
                    break;
                case 0x65E:
                    if ((unsigned short)(wparam >> 16) == 2 && SendMessageA((HWND)lparam, 0x18B, 0, 0) > 0) {
                        int selection = SendMessageA((HWND)lparam, 0x188, 0, 0);
                        if (selection != -1) {
                            SendMessageA((HWND)lparam, 0x4B7, selection, (LPARAM)dialog->Buffer);
                            HWND edit = GetDlgItem(window, 0x65D);
                            if (edit) {
                                SendMessageA(edit, 0x4B4, 0, (LPARAM)dialog->Buffer);
                                SetFocus(edit);
                            }
                        }
                    }
                    break;
                case 0x65D:
                    if ((unsigned short)(wparam >> 16) == 0x501) {
                        dialog->Add_Init_String_0066de80();
                    }
                    break;
                case 0x661:
                    if ((unsigned short)(wparam >> 16) == 0) {
                        dialog->Add_Init_String_0066de80();
                    }
                    break;
                case 0x660:
                    if ((unsigned short)(wparam >> 16) == 0) {
                        dialog->Delete_Init_String_0066dfe0();
                    }
                    break;
                case 0x65C:
                    if ((unsigned short)(wparam >> 16)) {
                        SendMessageA((HWND)lparam, 0xF1, 1, 0);
                        SendDlgItemMessageA(window, 0x65B, 0xF1, 0, 0);
                    }
                    break;
            }
            break;
    }
    return 0;
}


bool SerialEdDialog::Read_Settings_0066cf60()
{
    Debug_Printf("Reading serial settings dialog\n");
    HWND control = GetDlgItem(Window, 0x657);
    if (control) {
        int row = SendMessageA(control, CB_GETCURSEL, 0, 0);
        int data = SendMessageA(control, CB_GETITEMDATA, row, 0);
        Settings.Port = 1;
        Settings.ModemName[0] = '\0';
        switch (data) {
            case 0:
                Settings.Port = 0x3F8;
                break;
            case 1:
                Settings.Port = 0x2F8;
                break;
            case 2:
                Settings.Port = 0x3E8;
                break;
            case 3:
                Settings.Port = 0x2E8;
                break;
            case 4: {
                SendMessageA(control, 0x4BD, row, (LPARAM)Buffer);
                Buffer[62] = '\0';
                char *text = strchr(Buffer, '-');
                if (text) {
                    strcpy(Settings.ModemName, text + 1);
                }
                break;
            }
            default:
                SendMessageA(control, 0x4BD, row, (LPARAM)Buffer);
                Buffer[62] = '\0';
                strcpy(Settings.ModemName, Buffer);
                break;
        }
    }
    SerialSettingsType *settings = &Settings;
    Debug_Printf("Port: %lx\n", Settings.Port);
    Debug_Printf("Modem: %s\n", Settings.ModemName);
    control = GetDlgItem(Window, 0x658);
    if (control) {
        int row = SendMessageA(control, CB_GETCURSEL, 0, 0);
        int index = SendMessageA(control, CB_GETITEMDATA, row, 0);
        swscanf(unknown_baud_names_7f3070[index], L"%d", &Settings.Baud);
    }
    Debug_Printf("Baud: %ld\n", Settings.Baud);
    control = GetDlgItem(Window, 0x65E);
    if (control) {
        int selection = SendMessageA(control, 0x188, 0, 0);
        if (selection != -1 && unknown_session_a3d298.InitStrings.Count() != 0) {
            Settings.InitStringIndex = selection;
        } else {
            Settings.InitStringIndex = -1;
        }
    }
    control = GetDlgItem(Window, 0x659);
    if (control) {
        int row = SendMessageA(control, CB_GETCURSEL, 0, 0);
        Settings.CallWaitStringIndex = SendMessageA(control, CB_GETITEMDATA, row, 0);
        Settings.CallWaitString[0] = '\0';
        if (Settings.CallWaitStringIndex == 3) {
            SendMessageA(control, 0x4BD, row, (LPARAM)Buffer);
            Buffer[15] = '\0';
            char *text = strchr(Buffer, '-');
            if (text) {
                strcpy(Settings.CallWaitString, text + 1);
            }
        }
    }
    Settings.DialMethod = 0;
    control = GetDlgItem(Window, 0x65B);
    if (control && SendMessageA(control, 0xF0, 0, 0) == 1) {
        Settings.DialMethod = 1;
    }
    Debug_Printf("DialMethod: %s\n", Settings.DialMethod == 0 ? "Tone" : "Pulse");
    control = GetDlgItem(Window, 0x3EA);
    if (control) {
        if (SendMessageA(control, 0xF0, 0, 0) == 1) {
            Settings.ErrorCorrection = 1;
        } else {
            Settings.ErrorCorrection = 0;
        }
    }
    control = GetDlgItem(Window, 0x3EB);
    if (control) {
        if (SendMessageA(control, 0xF0, 0, 0) == 1) {
            Settings.Compression = 1;
        } else {
            Settings.Compression = 0;
        }
    }
    switch (serial_manager_00a40a28.Detect_Port(settings)) {
        case 0:
            return true;
        case 1:
            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_UNABLE_TO_OPEN_PORT", 0, "D:\\Ra2\\SerialEd.cpp", 0x213),
                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\SerialEd.cpp", 0x213), 0, 0, false);
            break;
        case 2:
            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_IRQ_ALREADY_IN_USE", 0, "D:\\Ra2\\SerialEd.cpp", 0x217),
                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\SerialEd.cpp", 0x217), 0, 0, false);
            break;
    }
    return false;
}


void SerialEdDialog::Init_0066d370(SerialSettingsType const *settings)
{
    Settings = *settings;
    if (Settings.Port == 0) {
        Settings.Port = 0x2F8;
    }
    if (Settings.IRQ == -1) {
        Settings.IRQ = 3;
    }
    if (Settings.Baud == -1) {
        Settings.Baud = 57600;
    }
    Fill_Port_0066d5a0();
    Fill_Baud_0066d860();
    Fill_Call_Wait_0066d930();
    Update_Radio_0066da70();
    if (unknown_session_a3d298.InitStrings.Count() == 0) {
        char *text = new char[8];
        strcpy(text, "ATZ");
        unknown_session_a3d298.InitStrings.Add(text);
        Settings.InitStringIndex = 0;
    }
    Fill_Init_Strings_0066d9b0();
    HWND button = GetDlgItem(Window, 0x3EB);
    if (button) {
        SendMessageA(button, 0xF1, settings->Compression != 0, 0);
    }
    button = GetDlgItem(Window, 0x3EA);
    if (button) {
        SendMessageA(button, 0xF1, settings->ErrorCorrection != 0, 0);
    }
}


// void SerialEdDialog::Fill_Port_0066d5a0() (0x0066D5A0)
// Fills the port combo box 0x657 (TibSun serialed.cpp
// SerialEdit::Build_Port_Listbox): COM1-COM4, the first ten registry modems
// (data 5..), the custom entry (data 4), then selects Settings.ModemName or
// Settings.Port, made the custom entry when it is not listed.
// Every message goes through an inline SendMessageA wrapper: its parameters are
// the extra (code-free) temporaries that give VC6 the original's scratch
// registers from the second modem-name address on (a plain call is 87.92).
static inline LRESULT Send_Message(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
{
    return SendMessageA(handle, message, wparam, lparam);
}

void SerialEdDialog::Fill_Port_0066d5a0()
{
    int i;
    int index;
    int port_index;
    wchar_t custom[128];
    wchar_t custom_port[128];

    HWND handle = GetDlgItem(Window, 0x657);
    if (handle) {
        Send_Message(handle, CB_RESETCONTENT, 0, 0);
        for (i = 0; i < 4; i++) {
            index = Send_Message(handle, 0x4C2, 0,
                (LPARAM)StringTable::Load_String(unknown_port_names_7f305c[i], 0, "D:\\Ra2\\SerialEd.cpp", 0x287));
            Send_Message(handle, CB_SETITEMDATA, index, i);
        }

        ModemCount = 0;
        for (i = 0; i < 10; i++) {
            ModemRegistryEntryClass *modemregistry = new ModemRegistryEntryClass(i);
            if (modemregistry) {
                if (modemregistry->ModemName) {
                    strncpy(ModemNames[ModemCount], modemregistry->ModemName, 63);
                    index = Send_Message(handle, 0x4BC, 0, (LPARAM)ModemNames[ModemCount]);
                    Send_Message(handle, CB_SETITEMDATA, index, ModemCount + 5);
                    ModemCount++;
                }
                delete modemregistry;
            }
        }

        wcscpy(custom, StringTable::Load_String("TXT_CUSTOM", 0, "D:\\Ra2\\SerialEd.cpp", 0x2A2));
        index = Send_Message(handle, 0x4C2, 0, (LPARAM)custom);
        Send_Message(handle, CB_SETITEMDATA, index, 4);
        Send_Message(handle, CB_SETCURSEL, 0, 0);

        if (strlen(Settings.ModemName)) {
            port_index = Send_Message(handle, 0x4B8, -1, (LPARAM)Settings.ModemName);
            if (port_index == -1) {
                port_index = Find_Combo_Item_Data_00736e10(handle, -1, 4);
                if (port_index != -1) {
                    swprintf(custom, L"%s %hs",
                        StringTable::Load_String("TXT_CUSTOM", 0, "D:\\Ra2\\SerialEd.cpp", 0x2B4), Settings.ModemName);
                    Send_Message(handle, 0x144, port_index, 0);
                    port_index = Send_Message(handle, 0x4C2, 0, (LPARAM)custom);
                    Send_Message(handle, CB_SETITEMDATA, port_index, 4);
                }
            }
            Send_Message(handle, CB_SETCURSEL, port_index, 0);
        } else {
            switch (Settings.Port) {
                case 0x3F8:
                    port_index = 0;
                    port_index = Find_Combo_Item_Data_00736e10(handle, -1, port_index);
                    break;
                case 0x2F8:
                    port_index = 1;
                    port_index = Find_Combo_Item_Data_00736e10(handle, -1, port_index);
                    break;
                case 0x3E8:
                    port_index = 2;
                    port_index = Find_Combo_Item_Data_00736e10(handle, -1, port_index);
                    break;
                case 0x2E8:
                    port_index = 3;
                    port_index = Find_Combo_Item_Data_00736e10(handle, -1, port_index);
                    break;
                default:
                    port_index = 4;
                    port_index = Find_Combo_Item_Data_00736e10(handle, -1, port_index);
                    if (port_index != -1) {
                        swprintf(custom_port, L"%s %x",
                            StringTable::Load_String("TXT_CUSTOM", 0, "D:\\Ra2\\SerialEd.cpp", 0x2DA), Settings.Port);
                        Send_Message(handle, 0x144, port_index, 0);
                        port_index = Send_Message(handle, 0x4C2, 0, (LPARAM)custom_port);
                        Send_Message(handle, CB_SETITEMDATA, port_index, 4);
                    }
                    break;
            }
            Send_Message(handle, CB_SETCURSEL, port_index, 0);
        }
    }
}


void SerialEdDialog::Fill_Baud_0066d860()
{
    HWND control = GetDlgItem(Window, 0x658);
    if (control) {
        SendMessageA(control, CB_RESETCONTENT, 0, 0);
        for (int index = 0; index < 6; index++) {
            unsigned short *name = unknown_baud_names_7f3070[index];
            int row = SendMessageA(control, 0x4C2, 0, (LPARAM)name);
            SendMessageA(control, CB_SETITEMDATA, row, index);
        }
        int selection;
        switch (Settings.Baud) {
            case 14400:
                selection = 0;
                break;
            case 19200:
                selection = 1;
                break;
            case 28800:
                selection = 2;
                break;
            default:
                selection = 3;
                break;
            case 57600:
                selection = 4;
                break;
            case 115200:
                selection = 5;
                break;
        }
        int row = Find_Combo_Item_Data_00736e10(control, -1, selection);
        SendMessageA(control, CB_SETCURSEL, row, 0);
    }
}


void SerialEdDialog::Fill_Call_Wait_0066d930()
{
    HWND control = GetDlgItem(Window, 0x659);
    if (control) {
        SendMessageA(control, CB_RESETCONTENT, 0, 0);
        for (int index = 0; index < 4; index++) {
            unsigned short const *text = unknown_session_a3d298.CallWaitStrings[index];
            int row = SendMessageA(control, 0x4C2, 0, (LPARAM)text);
            SendMessageA(control, CB_SETITEMDATA, row, index);
        }
        int row = Find_Combo_Item_Data_00736e10(control, -1, Settings.CallWaitStringIndex);
        SendMessageA(control, CB_SETCURSEL, row, 0);
    }
}


void SerialEdDialog::Fill_Init_Strings_0066d9b0()
{
    HWND control = GetDlgItem(Window, 0x65E);
    if (control) {
        SendMessageA(control, 0x184, 0, 0);
        for (int index = 0; index < unknown_session_a3d298.InitStrings.Count(); index++) {
            SendMessageA(control, 0x4C8, 0, (LPARAM)unknown_session_a3d298.InitStrings[index]);
        }
        if (SendMessageA(control, 0x18B, 0, 0) == 0 || Settings.InitStringIndex < -1) {
            Settings.InitStringIndex = -1;
        } else if (Settings.InitStringIndex >= SendMessageA(control, 0x18B, 0, 0)) {
            Settings.InitStringIndex = 0;
        }
        if (Settings.InitStringIndex != -1) {
            SendMessageA(control, 0x186, Settings.InitStringIndex, 0);
        }
    }
}


void SerialEdDialog::Update_Radio_0066da70()
{
    bool first = Settings.DialMethod == 0;
    HWND button = GetDlgItem(Window, 0x65C);
    if (button != 0) {
        SendMessageA(button, 0xF1, first == true, 0);
    }
    button = GetDlgItem(Window, 0x65B);
    if (button != 0) {
        SendMessageA(button, 0xF1, first == false, 0);
    }
}


void SerialEdDialog::Show_Text_0066dae0(int id)
{
    HWND control = GetDlgItem(Window, id);
    if (control != 0) {
        ShowWindow(control, SW_SHOW);
        EnableWindow(control, TRUE);
        char *text = strchr(Buffer, '-');
        if (text != 0) {
            text++;
            strtrim(text);
        } else {
            text = "";
        }
        SendMessageA(control, 0x4B4, 0, (LPARAM)text);
        SetFocus(control);
    }
}


// void SerialEdDialog::Read_Custom_Port_0066db50() (0x0066DB50)
// Takes the custom port typed into the hidden edit box 0x65F (TibSun
// serialed.cpp SerialEdit::Set_Port): a port address or COM1-COM4 selects that
// port in the combo box 0x657, other text becomes its custom entry, and a bad
// COM number shows TXT_INVALID_PORT_ADDRESS.
void SerialEdDialog::Read_Custom_Port_0066db50()
{
    int port_index = -1;

    HWND combo = GetDlgItem(Window, 0x657);
    HWND edit = GetDlgItem(Window, 0x65F);
    if (combo && edit) {
        ShowWindow(edit, SW_HIDE);
        EnableWindow(edit, FALSE);
        SendMessageA(edit, 0x4B5, 0x40, (LPARAM)Buffer);
        Buffer[63] = '\0';
        strtrim(Buffer);
        _strupr(Buffer);

        if (_stricmp(Buffer, "3F8") == 0) {
            port_index = 0;
        } else if (_stricmp(Buffer, "2F8") == 0) {
            port_index = 1;
        } else if (_stricmp(Buffer, "3E8") == 0) {
            port_index = 2;
        } else if (_stricmp(Buffer, "2E8") == 0) {
            port_index = 3;
        } else if (strncmp(Buffer, "COM", 3) == 0) {
            switch (Buffer[3]) {
                case '1':
                    port_index = 0;
                    break;
                case '2':
                    port_index = 1;
                    break;
                case '3':
                    port_index = 2;
                    break;
                case '4':
                    port_index = 3;
                    break;
                default:
                    if (Buffer[3] <= '9' && Buffer[3] > '0') {
                        Buffer[4] = '\0';
                        Set_Custom_Entry_0066dd60(0x657, 4, Buffer);
                    } else {
                        WWMessageBox(L" ").Process(
                            StringTable::Load_String("TXT_INVALID_PORT_ADDRESS", 0, "D:\\Ra2\\SerialEd.cpp", 0x440),
                            StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\SerialEd.cpp", 0x440), 0, 0, false);
                    }
            }
        } else {
            Set_Custom_Entry_0066dd60(0x657, 4, Buffer);
        }

        if (port_index != -1) {
            int row = Find_Combo_Item_Data_00736e10(combo, -1, port_index);
            SendMessageA(combo, CB_SETCURSEL, row, 0);
        }
    }
}


void SerialEdDialog::Read_Custom_Call_Wait_0066dcf0()
{
    HWND control = GetDlgItem(Window, 0x65A);
    if (control) {
        ShowWindow(control, SW_HIDE);
        EnableWindow(control, FALSE);
        SendMessageA(control, 0x4B5, 16, (LPARAM)Buffer);
        Buffer[15] = '\0';
        strtrim(Buffer);
        _strupr(Buffer);
        Set_Custom_Entry_0066dd60(0x659, 3, Buffer);
    }
}


void SerialEdDialog::Set_Custom_Entry_0066dd60(int id, int data, char const *text)
{
    char buffer[128];
    HWND control = GetDlgItem(Window, id);
    if (control) {
        int row = Find_Combo_Item_Data_00736e10(control, -1, data);
        SendMessageA(control, 0x4BD, row, (LPARAM)buffer);
        char *dash = strchr(buffer, '-');
        if (dash && _stricmp(text, dash + 1)) {
            sprintf(buffer, "%ls ", StringTable::Load_String("TXT_CUSTOM", 0, "D:\\Ra2\\SerialEd.cpp", 0x4A3));
            strcat(buffer, text);
            SendMessageA(control, 0x144, row, 0);
            row = SendMessageA(control, 0x4BC, 0, (LPARAM)buffer);
            SendMessageA(control, CB_SETITEMDATA, row, data);
            SendMessageA(control, CB_SETCURSEL, row, 0);
        }
    }
}


void SerialEdDialog::Add_Init_String_0066de80()
{
    HWND edit = GetDlgItem(Window, 0x65D);
    HWND list = GetDlgItem(Window, 0x65E);
    if (edit && list) {
        SendMessageA(edit, 0x4B5, sizeof(Buffer), (LPARAM)Buffer);
        SendMessageA(edit, 0x4B4, 0, (LPARAM)"");
        strtrim(Buffer);
        if (strlen(Buffer)) {
            _strupr(Buffer);
            int index = SendMessageA(list, 0x4C4, -1, (LPARAM)Buffer);
            if (index == -1) {
                char *text = new char[41];
                strcpy(text, Buffer);
                index = SendMessageA(list, 0x4C8, 0, (LPARAM)text);
                if (index == -1) {
                    return;
                }
                unknown_session_a3d298.InitStrings.Add(text);
            }
            SendMessageA(list, 0x186, index, 0);
            SendMessageA(list, 0x197, index, 0);
            Settings.InitStringIndex = index;
        }
    }
}


void SerialEdDialog::Delete_Init_String_0066dfe0()
{
    HWND list = GetDlgItem(Window, 0x65E);
    if (list && SendMessageA(list, 0x18B, 0, 0) > 0) {
        int index = SendMessageA(list, 0x188, 0, 0);
        if (index != -1) {
            SendMessageA(list, 0x182, index, 0);
            unknown_session_a3d298.InitStrings.Delete(index);
            if (index >= SendMessageA(list, 0x18B, 0, 0)) {
                index--;
            }
            SendMessageA(list, 0x186, index, 0);
            SendMessageA(list, 0x197, index, 0);
            Settings.InitStringIndex = index;
        }
    }
}
