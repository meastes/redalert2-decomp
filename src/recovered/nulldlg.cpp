// nulldlg.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/nulldlg.json carries the addresses and the rows that
// stay in their own files.


#define RA2_TTIMER_CONVERSION_INT
#define RA2_CDTIMER_CONVERSION_BODY
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_OBJECT
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#include "shared/null_dlg.hpp"
#include "shared/session_state.hpp"
#include "shared/serial_progress.hpp"
#include "shared/win_modem.hpp"
#include "shared/win32_window.hpp"
#include "shared/dialog_create.hpp"
#include "shared/ms_message_box.hpp"
#include "shared/ww_message_box.hpp"
#include "shared/string_table.hpp"
#include "shared/comm_buffer.hpp"
#include "shared/cd_timer.hpp"
#include "shared/queue.hpp"
#include "shared/crt_string.hpp"
#include "shared/phone_edit.hpp"
#include "shared/serial_ed.hpp"
#include "shared/multi_mission.hpp"
#include "shared/modem_host.hpp"
#include "shared/modem_guest.hpp"
#include "shared/dsurface.hpp"
#define CLRDTR 6
#include "shared/event.hpp"
#include "shared/house.hpp"
#include "shared/house_type.hpp"
#include "shared/rules.hpp"
#include "shared/map_view.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/gadget.hpp"
#include "shared/vector_class.hpp"
#include "shared/debug_printf.hpp"


// Header-static dynamic initializers of the unit at 0x005CDD10: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x005CDD10-0x005CDEF1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x005CDD30, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_005cdd10.cpp; no source of its own.


// Dynamic initializer (0x005CDD60, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_005cdd10.cpp; no source of its own.


// Dynamic initializer (0x005CDD80, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_005cdd10.cpp; no source of its own.


// Dynamic initializer (0x005CDDA0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_005cdd10.cpp; no source of its own.


// Dynamic initializer (0x005CDDC0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_005cdd10.cpp; no source of its own.


// Dynamic initializer (0x005CDDE0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_005cdd10.cpp; no source of its own.


// Dynamic initializer (0x005CDE00, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_005cdd10.cpp; no source of its own.


// Dynamic initializer (0x005CDE40, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_005cdd10.cpp; no source of its own.


// Dynamic initializer (0x005CDE70, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_005cdd10.cpp; no source of its own.


// Dynamic initializer (0x005CDEA0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_005cdd10.cpp; no source of its own.


// Dynamic initializer (0x005CDED0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_005cdd10.cpp; no source of its own.


// Dynamic initializer (0x005CDEE0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_005cdd10.cpp; no source of its own.


// int __fastcall Select_Serial_Dialog() (0x005CDF00)
// The serial game dialog (Red Alert's Select_Serial_Dialog; statement shape from the TibSun
// reconstruction nulldlg.cpp): settings edits the session's default port, dial and answer open a
// modem link and run the modem host or guest dialog, null modem tests the cable (Test_Null_Modem,
// inlined) and runs the host or guest dialog by who won. Returns the game type (0: none).
// The unit's first include of shared/vector_class.hpp is here (through queue.hpp): Destroy_Null_Connection
// (0x005CEBA0) inlines DynamicVectorClass<NodeNameType *>::Delete(T const &).

WIN32_IMPORT BOOL WINAPI SetCommBreak(HANDLE file);
WIN32_IMPORT BOOL WINAPI ClearCommBreak(HANDLE file);

void __fastcall Unknown_00517370(bool unknown);
extern Surface *unknown_surface_839954;

static int Test_Null_Modem()
{
    int retval = 0;
    bool process = true;
    unsigned int starttime;
    int packetlen;
    char buffer[80 * 3];
    UnknownSerialPacket SendPacket;
    UnknownSerialPacket ReceivePacket;

    SetCommBreak(SerialPort->Get_Port_Handle());
    SerialPort->Write_To_Serial_Port((unsigned char *)"ATH\r", strlen("ATH\r"));
    CDTimerClass<SystemTimerClass> time;
    time = 120;
    while (time) {
    }
    ClearCommBreak(SerialPort->Get_Port_Handle());
    EscapeCommFunction(SerialPort->Get_Port_Handle(), CLRDTR);

    starttime = TickCount.Time();
    while (TickCount.Time() - starttime < 80) {
        serial_manager_00a40a28.Service();
        if (serial_manager_00a40a28.Get_Message(&ReceivePacket, &packetlen) > 0) {
            if (ReceivePacket.Command == 100) {
                starttime = TickCount.Time();
                while (TickCount.Time() - starttime < 30) {
                    serial_manager_00a40a28.Service();
                }
                process = false;
                retval = 2;
                break;
            }
        }
    }

    memset(&SendPacket, 0, sizeof(UnknownSerialPacket));
    if (process == true) {
        SendPacket.Command = 100;
        SendPacket.Options.Seed = TickCount;
        SendPacket.ID = (int)buffer;
        serial_manager_00a40a28.Send_Message(&SendPacket, sizeof(SendPacket), 1);
        starttime = TickCount;
        while (TickCount - starttime < 80) {
            serial_manager_00a40a28.Service();
            if (serial_manager_00a40a28.Get_Message(&ReceivePacket, &packetlen) > 0) {
                if (ReceivePacket.Command == 100) {
                    starttime = TickCount;
                    while (TickCount - starttime < 30) {
                        serial_manager_00a40a28.Service();
                    }
                    if (ReceivePacket.Options.Seed > SendPacket.Options.Seed) {
                        process = false;
                        retval = 2;
                    } else if (ReceivePacket.Options.Seed == SendPacket.Options.Seed) {
                        if (ReceivePacket.ID > SendPacket.ID) {
                            process = false;
                            retval = 2;
                        } else if (ReceivePacket.ID == SendPacket.ID) {
                            process = false;
                            retval = 3;
                        }
                    }
                    break;
                }
            }
        }

        if (process) {
            unknown_modem_aborted_a72ec4 = false;
            HWND dialog = unknown_cancel_dialog_6001e0(
                StringTable::Load_String("TXT_WAITING_CONNECT", 0, "D:\\Ra2\\NullDlg.CPP", 0x1895),
                StringTable::Load_String("TXT_CANCEL", 0, "D:\\Ra2\\NullDlg.CPP", 0x1895), &unknown_modem_aborted_a72ec4);
            if (dialog) {
                unknown_dialog_5ff7c0(dialog);
                starttime = TickCount;
                while (true) {
                    if (unknown_6000d0() == true || unknown_modem_aborted_a72ec4) {
                        retval = 0;
                        break;
                    }
                    Unknown_00517370(false);
                    serial_manager_00a40a28.Service();
                    if (serial_manager_00a40a28.Num_Send() == 0) {
                        if (serial_manager_00a40a28.Get_Message(&ReceivePacket, &packetlen) > 0) {
                            if (ReceivePacket.Command == 100) {
                                starttime = TickCount;
                                while (TickCount - starttime < 30) {
                                    serial_manager_00a40a28.Service();
                                }
                                if (ReceivePacket.Options.Seed > SendPacket.Options.Seed) {
                                    process = false;
                                    retval = 2;
                                } else if (ReceivePacket.Options.Seed == SendPacket.Options.Seed) {
                                    if (ReceivePacket.ID > SendPacket.ID) {
                                        process = false;
                                        retval = 2;
                                    } else if (ReceivePacket.ID == SendPacket.ID) {
                                        process = false;
                                        retval = 3;
                                    }
                                }
                            } else {
                                retval = 0;
                                process = false;
                            }
                        } else {
                            retval = 1;
                            process = false;
                        }
                    }
                    if (TickCount - starttime > 3600) {
                        retval = 0;
                        break;
                    }
                    if (process != true) {
                        break;
                    }
                }
                unknown_dialog_5ff6e0(dialog);
            }
        }
    }
    return retval;
}

int __fastcall Select_Serial_Dialog()
{
    int rc = -1;
    int retval = -1;
    SerialSettingsType *settings;

    HWND dialog = unknown_create_dialog_5ff610(0xEC, Select_Serial_Dialog_Proc, 0);
    if (dialog) {
        SetWindowLongA(dialog, DWL_USER, (LONG)&rc);
        unknown_move_window_600120(dialog, -1, ((unknown_surface_839954->Get_Height() - 400) / 2) + 147);
        unknown_dialog_5ff7c0(dialog);

        if (unknown_session_a3d298.SerialDefaults.Port == 0 || unknown_session_a3d298.SerialDefaults.IRQ == -1
            || unknown_session_a3d298.SerialDefaults.Baud == -1) {
            rc = 0x652;
        } else if (serial_manager_00a40a28.Detect_Port(&unknown_session_a3d298.SerialDefaults) != 0) {
            rc = 0x652;
        }

        while (retval == -1) {
            while (rc < 0) {
                if (unknown_6000d0() == true) {
                    break;
                }
                Unknown_00517370(false);
            }

            switch (rc) {
            case 0x653:
                ShowWindow(dialog, SW_HIDE);
                UpdateWindow(MainWindow);
                if (PhoneEdit().Dialog() == true) {
                    if (unknown_session_a3d298.PhoneBook[unknown_session_a3d298.CurPhoneIdx]->Settings.Port == 0) {
                        settings = &unknown_session_a3d298.SerialDefaults;
                    } else {
                        settings = &unknown_session_a3d298.PhoneBook[unknown_session_a3d298.CurPhoneIdx]->Settings;
                    }
                    if (SerialPort) {
                        delete SerialPort;
                    }
                    SerialPort = new WinModemClass;
                    if (serial_manager_00a40a28.Init(settings->Port, settings->IRQ, settings->ModemName, settings->Baud, 0, 8, 1)) {
                        if (settings->CallWaitStringIndex == 3) {
                            strcpy(DialString, settings->CallWaitString);
                        } else {
                            WideToCharString_006fa0f0(DialString, unknown_session_a3d298.CallWaitStrings[settings->CallWaitStringIndex], -1);
                        }
                        strcat(DialString, unknown_session_a3d298.PhoneBook[unknown_session_a3d298.CurPhoneIdx]->Number);
                        if (Dial_Modem(settings, false)) {
                            serial_local_player_00a3fb38 = 2;
                            unknown_game_mode_a3d298 = 1;
                            if (ModemHost().Dialog_00598e10()) {
                                retval = 1;
                                break;
                            }
                            unknown_game_mode_a3d298 = 0;
                            retval = 0;
                        }
                        serial_manager_00a40a28.Change_IRQ_Priority(0);
                    } else {
                        WWMessageBox(L" ").Process(StringTable::Load_String("TXT_SELECT_SETTINGS", 0, "D:\\Ra2\\NullDlg.CPP", 0x16A6),
                            StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x16A6), 0, 0, false);
                    }
                }
                ShowWindow(dialog, SW_SHOW);
                break;

            case 0x654:
                ShowWindow(dialog, SW_HIDE);
                UpdateWindow(MainWindow);
                settings = &unknown_session_a3d298.SerialDefaults;
                if (SerialPort) {
                    delete SerialPort;
                }
                SerialPort = new WinModemClass;
                if (serial_manager_00a40a28.Init(settings->Port, settings->IRQ, settings->ModemName, settings->Baud, 0, 8, 1)) {
                    if (Answer_Modem(settings, false)) {
                        serial_local_player_00a3fb38 = 3;
                        unknown_game_mode_a3d298 = 1;
                        if (ModemGuest().Dialog_00596010()) {
                            retval = 1;
                            break;
                        }
                        unknown_game_mode_a3d298 = 0;
                        retval = 0;
                    }
                    serial_manager_00a40a28.Change_IRQ_Priority(0);
                } else {
                    WWMessageBox(L" ").Process(StringTable::Load_String("TXT_SELECT_SETTINGS", 0, "D:\\Ra2\\NullDlg.CPP", 0x16C9),
                        StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x16C9), 0, 0, false);
                }
                ShowWindow(dialog, SW_SHOW);
                break;

            case 0x655:
                ShowWindow(dialog, SW_HIDE);
                UpdateWindow(MainWindow);
                if (serial_manager_00a40a28.Init(unknown_session_a3d298.SerialDefaults.Port, unknown_session_a3d298.SerialDefaults.IRQ, unknown_session_a3d298.SerialDefaults.ModemName, unknown_session_a3d298.SerialDefaults.Baud, 0, 8, 1)) {
                    switch (Test_Null_Modem()) {
                    case 1:
                        serial_local_player_00a3fb38 = 0;
                        unknown_game_mode_a3d298 = 2;
                        if (ModemHost().Dialog_00598e10() == 1) {
                            retval = 2;
                            break;
                        }
                        unknown_game_mode_a3d298 = 0;
                        break;
                    case 2:
                        serial_local_player_00a3fb38 = 1;
                        unknown_game_mode_a3d298 = 2;
                        if (ModemGuest().Dialog_00596010() == 1) {
                            retval = 2;
                            break;
                        }
                        unknown_game_mode_a3d298 = 0;
                        break;
                    case 3:
                        WWMessageBox(L" ").Process(StringTable::Load_String("TXT_MODEM_OR_LOOPBACK", 0, "D:\\Ra2\\NullDlg.CPP", 0x16EC),
                            StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x16EC), 0, 0, false);
                        break;
                    }
                    if (retval == -1) {
                        serial_manager_00a40a28.Change_IRQ_Priority(0);
                        ShowWindow(dialog, SW_SHOW);
                    }
                } else {
                    WWMessageBox(L" ").Process(StringTable::Load_String("TXT_SELECT_SETTINGS", 0, "D:\\Ra2\\NullDlg.CPP", 0x16F5),
                        StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x16F5), 0, 0, false);
                    ShowWindow(dialog, SW_SHOW);
                }
                break;

            case 0x652:
                ShowWindow(dialog, SW_HIDE);
                UpdateWindow(MainWindow);
                if (SerialEdDialog().Dialog_0066ca70(&unknown_session_a3d298.SerialDefaults)) {
                    unknown_session_a3d298.Write_MultiPlayer_Settings();
                }
                Select_Serial_Dialog_Init(dialog);
                ShowWindow(dialog, SW_SHOW);
                break;

            default:
                retval = 0;
                break;
            }
            rc = -1;
        }
        unknown_dialog_5ff6e0(dialog);
        return retval;
    }
    return 0;
}


// int __stdcall Select_Serial_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) (0x005CE920)
// The shared handler first; 0x497 initialises the buttons, WM_COMMAND stores
// IDCANCEL or a button 0x652-0x655 through the DWL_USER pointer.
// The unit's first include of shared/vector_class.hpp is here: Destroy_Null_Connection
// needs the accessors and the DynamicVectorClass Delete members inline.

int __stdcall Select_Serial_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int handled = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (handled != 0) {
        return handled;
    }
    switch (message) {
        case 0x497:
            Select_Serial_Dialog_Init(window);
            return 0;
        case 0x111: {
            int *result = (int *)GetWindowLongA(window, 8);
            if (wparam == 2 || (wparam > 0x651 && wparam <= 0x655)) {
                *result = wparam;
            }
            break;
        }
    }
    return 0;
}


// void __fastcall Select_Serial_Dialog_Init(HWND window) (0x005CE990)
// Enables the buttons 0x653-0x655 only when the default port settings are
// complete and the port opens (the session's SerialDefaults).

void __fastcall Select_Serial_Dialog_Init(HWND window)
{
    bool enable = true;
    if (unknown_session_a3d298.SerialDefaults.Port == 0 || unknown_session_a3d298.SerialDefaults.IRQ == -1 || unknown_session_a3d298.SerialDefaults.Baud == -1) {
        enable = false;
    } else if (serial_manager_00a40a28.Detect_Port(&unknown_session_a3d298.SerialDefaults) != 0) {
        enable = false;
    }

    HWND item = GetDlgItem(window, 0x653);
    if (item) {
        EnableWindow(item, enable);
    }
    item = GetDlgItem(window, 0x654);
    if (item) {
        EnableWindow(item, enable);
    }
    item = GetDlgItem(window, 0x655);
    if (item) {
        EnableWindow(item, enable);
    }
}


// void Shutdown_Modem() (0x005CEA60)
// Hangs a modem game's line up unless the game is playing, then closes the port.

void Shutdown_Modem()
{
    if (!(unknown_flags_a3f600 & 2)) {
        if (unknown_game_mode_a3d298 == 1) {
            serial_manager_00a40a28.Hangup_Modem();
        }
    }
    serial_manager_00a40a28.Shutdown();
}


// void __fastcall Modem_Signoff() (0x005CEA90)
// Unless the 0x02 flag of 0x00A3F600 is set, sends the other side an exit
// event (type 0x13) twice and services the link for 30 ticks (Red Alert's
// Modem_Signoff, which sends EventClass::EXIT the same way).

void __fastcall Modem_Signoff()
{
    EventClass event;

    if (!(unknown_flags_a3f600 & 2)) {
        memset(&event, 0, sizeof(event));
        event.Type = 0x13;
        serial_manager_00a40a28.Send_Message(&event, sizeof(event), 0);
        serial_manager_00a40a28.Send_Message(&event, sizeof(event), 0);
        unsigned long starttime = TickCount.Time();
        while (TickCount.Time() - starttime < 30) {
            serial_manager_00a40a28.Service();
        }
    }
}


// bool Reconnect_Modem() (0x005CEB40)
// Restores a lost serial connection by the session's modem role: a null
// modem, or a modem that still has carrier, reconnects; otherwise the dialer
// redials and the answerer answers again.

bool Reconnect_Modem()
{
    bool status = false;
    int modemstatus;

    switch (serial_local_player_00a3fb38) {
        case 0:
        case 1:
            status = Reconnect_Null_Modem();
            break;
        case 2:
            modemstatus = serial_manager_00a40a28.Get_Modem_Status();
            if (modemstatus & 0x80) {
                status = Reconnect_Null_Modem();
            } else {
                status = Dial_Modem(DialSettings, true);
            }
            break;
        case 3:
            modemstatus = serial_manager_00a40a28.Get_Modem_Status();
            if (modemstatus & 0x80) {
                status = Reconnect_Null_Modem();
            } else {
                status = Answer_Modem(DialSettings, true);
            }
            break;
    }
    return status;
}


// void __fastcall Destroy_Null_Connection(int id, int error) (0x005CEBA0)
// A serial player has gone: shows "left the game" (error 0) or "connection
// lost" (error 1), or drops the modem connection (error -1); removes the
// player from the session list, hands the house to the computer and, when
// only one player is left, says so.

void __fastcall Destroy_Null_Connection(int id, int error)
{
    int i;
    HouseClass *housep;
    wchar_t txt[80];

    if (unknown_session_a3d55c == 1) {
        return;
    }
    housep = Houses[id];
    if (!housep || !housep->unknown_134) {
        return;
    }

    txt[0] = 0;
    switch (error) {
        case 1:
            swprintf(txt, StringTable::Load_String("TXT_CONNECTION_LOST", 0, "D:\\Ra2\\NullDlg.CPP", 0x190C), housep->UIName);
            break;
        case 0:
            swprintf(txt, StringTable::Load_String("TXT_LEFT_GAME", 0, "D:\\Ra2\\NullDlg.CPP", 0x1910), housep->UIName);
            break;
        case -1:
            serial_manager_00a40a28.Delete_Connection();
            break;
    }

    if (wcslen(txt)) {
        unknown_session_a3d298.Messages.Add_Message(0, 0, txt, housep->Type->Color, 0x4046, Rules->MessageDelay * 900, false);
        Map.Flag_To_Redraw(false);
    }

    for (i = 0; i < unknown_session_a3d298.Players.Count(); i++) {
        if (!_wcsicmp(unknown_session_a3d298.Players[i]->Name, housep->UIName)) {
            delete unknown_session_a3d298.Players[i];
            unknown_session_a3d298.Players.Delete(unknown_session_a3d298.Players[i]);
            break;
        }
    }

    housep->AI_Takeover();
    unknown_session_a3d55c--;
    if (unknown_session_a3d55c == 1) {
        swprintf(txt, L"%s", StringTable::Load_String("TXT_JUST_YOU_AND_ME", 0, "D:\\Ra2\\NullDlg.CPP", 0x1935));
        unknown_session_a3d298.Messages.Add_Message(0, 0, txt, housep->Type->Color, 0x4046, Rules->MessageDelay * 900, false);
        Map.Flag_To_Redraw(false);
    }
}


// bool __fastcall Reconnect_Null_Modem() (0x005CEDD0)
// Re-establishes a null-modem link under a "check the cables" dialog: sends
// a connect packet with the local colour every 30 ticks until the other side
// answers with its own (then acknowledges it and services the link for 60
// ticks); the same colour means the other system does not respond. Gives up
// after 900 ticks without a message, or on cancel.

bool __fastcall Reconnect_Null_Modem()
{
    bool process = true;
    bool retval = false;
    unsigned long starttime;
    unsigned long lastmsgtime;
    int packetlen;
    UnknownSerialPacket SendPacket;
    UnknownSerialPacket ReceivePacket;

    unknown_modem_aborted_a72ec4 = false;
    HWND dialog = unknown_cancel_dialog_6001e0(
        StringTable::Load_String("TXT_NULL_CONNERR_CHECK_CABLES", 0, "D:\\Ra2\\NullDlg.CPP", 0x1959),
        StringTable::Load_String("TXT_CANCEL", 0, "D:\\Ra2\\NullDlg.CPP", 0x1959), &unknown_modem_aborted_a72ec4);
    if (dialog) {
        unknown_dialog_5ff7c0(dialog);
        starttime = lastmsgtime = TickCount.Time();
        while (process == true) {
            if (unknown_6000d0() == true || unknown_modem_aborted_a72ec4) {
                retval = false;
                break;
            }
            serial_manager_00a40a28.Service();
            if (TickCount.Time() - starttime > 30) {
                starttime = TickCount.Time();
                memset(&SendPacket, 0, sizeof(SendPacket));
                SendPacket.Command = 100;
                SendPacket.ID = unknown_a3d3b4;
                serial_manager_00a40a28.Send_Message(&SendPacket, sizeof(SendPacket), 0);
            }
            if (serial_manager_00a40a28.Get_Message(&ReceivePacket, &packetlen) > 0) {
                lastmsgtime = TickCount.Time();
                if (ReceivePacket.Command == 100) {
                    if (ReceivePacket.ID == unknown_a3d3b4) {
                        WWMessageBox(L" ").Process(StringTable::Load_String("TXT_SYSTEM_NOT_RESPONDING", 0, "D:\\Ra2\\NullDlg.CPP", 0x1982),
                            StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x1982), 0, 0, false);
                        retval = false;
                        break;
                    }
                    memset(&SendPacket, 0, sizeof(SendPacket));
                    SendPacket.Command = 100;
                    SendPacket.ID = unknown_a3d3b4;
                    serial_manager_00a40a28.Send_Message(&SendPacket, sizeof(SendPacket), 1);
                    starttime = TickCount.Time();
                    while (TickCount.Time() - starttime < 60) {
                        serial_manager_00a40a28.Service();
                    }
                    retval = true;
                    process = false;
                }
            }
            if (TickCount.Time() - lastmsgtime > 900) {
                retval = false;
                process = false;
            }
        }
        unknown_dialog_5ff6e0(dialog);
    }
    return retval;
}


// bool __fastcall Dial_Modem(SerialSettingsType *settings, bool reconnect) (0x005CF0B0)
// Dials DialString: logs the modem status lines; on reconnect an existing
// carrier is enough, otherwise a live line is hung up first. Detects the modem
// (a 14400 or 28800 modem is retried at 19200 or 38400), then dials and
// reports the result (a cancelled call is hung up). Modem servicing is off meanwhile.

bool __fastcall Dial_Modem(SerialSettingsType *settings, bool reconnect)
{
    bool connected = false;
    DialStatusType dialstatus;
    int modemstatus;

    Debug_Printf("Dial Modem\n");
    serial_session_flags_00a3fac0 &= ~1;
    DialSettings = settings;
    modemstatus = serial_manager_00a40a28.Get_Modem_Status();
    Debug_Printf("Get_Modem_Status indicates the following...\n");
    Debug_Printf("CTS: %s\n", (modemstatus & 0x10) ? "SET" : "CLEAR");
    Debug_Printf("DSR: %s\n", (modemstatus & 0x20) ? "SET" : "CLEAR");
    Debug_Printf(" RI: %s\n", (modemstatus & 0x40) ? "SET" : "CLEAR");
    Debug_Printf(" CD: %s\n", (modemstatus & 0x80) ? "SET" : "CLEAR");

    if (reconnect) {
        if (modemstatus & 0x80) {
            connected = true;
            serial_session_flags_00a3fac0 |= 1;
            return connected;
        }
    } else if (modemstatus & 0x80) {
        Debug_Printf("Modem already on-line - hanging up\n");
        serial_manager_00a40a28.Hangup_Modem();
        serial_session_flags_00a3fac0 &= ~1;
    }

    serial_manager_00a40a28.Setup_Modem_Echo(unknown_text_buffer_append_005cfd70);
    modemstatus = serial_manager_00a40a28.Detect_Modem(settings, reconnect);
    if (!modemstatus) {
        Debug_Printf("Detect_Modem failed\n");
        serial_manager_00a40a28.unknown_5d1d40();
        serial_manager_00a40a28.Translate_Line_Ends();
        serial_manager_00a40a28.Clear_Receive_Buffer();
        switch (settings->Baud) {
            case 14400:
                Debug_Printf("Upshifting to 19200\n");
                settings->Baud = 19200;
                Shutdown_Modem();
                serial_manager_00a40a28.Init(settings->Port, settings->IRQ, settings->ModemName, settings->Baud, 0, 8, 1);
                serial_manager_00a40a28.Setup_Modem_Echo(unknown_text_buffer_append_005cfd70);
                modemstatus = serial_manager_00a40a28.Detect_Modem(settings, reconnect);
                if (!modemstatus) {
                    Debug_Printf("Detect_Modem failed at 19200\n");
                    serial_manager_00a40a28.unknown_5d1d40();
                    serial_manager_00a40a28.Translate_Line_Ends();
                    serial_manager_00a40a28.Clear_Receive_Buffer();
                    WWMessageBox(L" ").Process(StringTable::Load_String("TXT_UNABLE_FIND_MODEM", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A08),
                        StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A08), 0, 0, false);
                    serial_session_flags_00a3fac0 |= 1;
                    return connected;
                }
                break;
            case 28800:
                Debug_Printf("Upshifting to 38400\n");
                settings->Baud = 38400;
                Shutdown_Modem();
                serial_manager_00a40a28.Init(settings->Port, settings->IRQ, settings->ModemName, settings->Baud, 0, 8, 1);
                serial_manager_00a40a28.Setup_Modem_Echo(unknown_text_buffer_append_005cfd70);
                modemstatus = serial_manager_00a40a28.Detect_Modem(settings, reconnect);
                if (!modemstatus) {
                    Debug_Printf("Detect_Modem failed at 38400\n");
                    serial_manager_00a40a28.unknown_5d1d40();
                    serial_manager_00a40a28.Translate_Line_Ends();
                    serial_manager_00a40a28.Clear_Receive_Buffer();
                    WWMessageBox(L" ").Process(StringTable::Load_String("TXT_UNABLE_FIND_MODEM", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A1D),
                        StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A1D), 0, 0, false);
                    serial_session_flags_00a3fac0 |= 1;
                    return connected;
                }
                break;
            default:
                WWMessageBox(L" ").Process(StringTable::Load_String("TXT_UNABLE_FIND_MODEM", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A24),
                    StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A24), 0, 0, false);
                serial_session_flags_00a3fac0 |= 1;
                return connected;
        }
    } else if (modemstatus == -1) {
        serial_manager_00a40a28.unknown_5d1d40();
        serial_manager_00a40a28.Translate_Line_Ends();
        serial_manager_00a40a28.Clear_Receive_Buffer();
        WWMessageBox(L" ").Process(StringTable::Load_String("TXT_ERROR_IN_INITSTRING", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A31),
            StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A31), 0, 0, false);
        serial_session_flags_00a3fac0 |= 1;
        return connected;
    }

    Debug_Printf("Ready to dial\n");
    dialstatus = serial_manager_00a40a28.Dial_Modem(DialString, settings->DialMethod, reconnect);
    if (reconnect) {
        Map.Flag_To_Redraw(2);
        Map.Render();
    }
    switch (dialstatus) {
        case DIAL_CONNECTED:
            Debug_Printf("Dial_Modem returned 'DIAL_CONNECTED'\n");
            connected = true;
            break;
        case DIAL_NO_CARRIER:
            Debug_Printf("Dial_Modem returned 'DIAL_NO_CARRIER'\n");
            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_NO_CARRIER", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A66),
                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A66), 0, 0, false);
            connected = false;
            break;
        case DIAL_BUSY:
            Debug_Printf("Dial_Modem returned 'DIAL_BUSY'\n");
            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_LINE_BUSY", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A6C),
                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A6C), 0, 0, false);
            connected = false;
            break;
        case DIAL_ERROR:
            Debug_Printf("Dial_Modem returned 'DIAL_ERROR'\n");
            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_NUMBER_INVALID", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A72),
                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A72), 0, 0, false);
            connected = false;
            break;
        case DIAL_TIMEOUT:
            Debug_Printf("Dial_Modem returned 'DIAL_TIMEOUT'\n");
            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_ERROR_TIMEOUT", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A78),
                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A78), 0, 0, false);
            connected = false;
            break;
        case DIAL_NO_DIAL_TONE:
            Debug_Printf("Dial_Modem returned 'DIAL_NO_DIAL_TONE'\n");
            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_NO_DIAL_TONE", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A7E),
                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A7E), 0, 0, false);
            connected = false;
            break;
        case DIAL_CANCELED:
            Debug_Printf("Dial_Modem returned 'DIAL_CANCELLED'\n");
            serial_manager_00a40a28.Hangup_Modem();
            serial_session_flags_00a3fac0 &= ~1;
            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_DIALING_CANCELED", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A86),
                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x1A86), 0, 0, false);
            connected = false;
            break;
    }

    serial_manager_00a40a28.unknown_5d1d40();
    serial_manager_00a40a28.Translate_Line_Ends();
    serial_manager_00a40a28.Clear_Receive_Buffer();
    serial_session_flags_00a3fac0 |= 1;
    return connected;
}


// bool __fastcall Answer_Modem(SerialSettingsType *settings, bool reconnect) (0x005CF780)
// Answers a modem call: logs the modem status lines; on reconnect an existing
// carrier is enough, otherwise a live line is hung up first. Detects the modem
// (a 14400 or 28800 modem is retried at 19200 or 38400), then waits for the
// call and reports the result. Modem servicing is off meanwhile.

bool __fastcall Answer_Modem(SerialSettingsType *settings, bool reconnect)
{
    bool connected = false;
    DialStatusType dialstatus;
    int modemstatus;

    Debug_Printf("Answer Modem\n");
    serial_session_flags_00a3fac0 &= ~1;
    DialSettings = settings;
    modemstatus = serial_manager_00a40a28.Get_Modem_Status();
    Debug_Printf("Get_Modem_Status indicates the following...\n");
    Debug_Printf("CTS: %s\n", (modemstatus & 0x10) ? "SET" : "CLEAR");
    Debug_Printf("DSR: %s\n", (modemstatus & 0x20) ? "SET" : "CLEAR");
    Debug_Printf(" RI: %s\n", (modemstatus & 0x40) ? "SET" : "CLEAR");
    Debug_Printf(" CD: %s\n", (modemstatus & 0x80) ? "SET" : "CLEAR");

    if (reconnect) {
        if (modemstatus & 0x80) {
            connected = true;
            serial_session_flags_00a3fac0 |= 1;
            return connected;
        }
    } else if (modemstatus & 0x80) {
        Debug_Printf("Modem already on-line - hanging up\n");
        serial_manager_00a40a28.Hangup_Modem();
        serial_session_flags_00a3fac0 &= ~1;
    }

    serial_manager_00a40a28.Setup_Modem_Echo(unknown_text_buffer_append_005cfd70);
    modemstatus = serial_manager_00a40a28.Detect_Modem(settings, reconnect);
    if (!modemstatus) {
        Debug_Printf("Detect_Modem failed\n");
        serial_manager_00a40a28.unknown_5d1d40();
        serial_manager_00a40a28.Translate_Line_Ends();
        serial_manager_00a40a28.Clear_Receive_Buffer();
        switch (settings->Baud) {
            case 14400:
                Debug_Printf("Upshifting to 19200\n");
                settings->Baud = 19200;
                Shutdown_Modem();
                serial_manager_00a40a28.Init(settings->Port, settings->IRQ, settings->ModemName, settings->Baud, 0, 8, 1);
                serial_manager_00a40a28.Setup_Modem_Echo(unknown_text_buffer_append_005cfd70);
                modemstatus = serial_manager_00a40a28.Detect_Modem(settings, reconnect);
                if (!modemstatus) {
                    Debug_Printf("Detect_Modem failed at 19200\n");
                    serial_manager_00a40a28.unknown_5d1d40();
                    serial_manager_00a40a28.Translate_Line_Ends();
                    serial_manager_00a40a28.Clear_Receive_Buffer();
                    WWMessageBox(L" ").Process(StringTable::Load_String("TXT_UNABLE_FIND_MODEM", 0, "D:\\Ra2\\NullDlg.CPP", 0x1AE7),
                        StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x1AE7), 0, 0, false);
                    serial_session_flags_00a3fac0 |= 1;
                    return connected;
                }
                break;
            case 28800:
                Debug_Printf("Upshifting to 38400\n");
                settings->Baud = 38400;
                Shutdown_Modem();
                serial_manager_00a40a28.Init(settings->Port, settings->IRQ, settings->ModemName, settings->Baud, 0, 8, 1);
                serial_manager_00a40a28.Setup_Modem_Echo(unknown_text_buffer_append_005cfd70);
                modemstatus = serial_manager_00a40a28.Detect_Modem(settings, reconnect);
                if (!modemstatus) {
                    Debug_Printf("Detect_Modem failed at 38400\n");
                    serial_manager_00a40a28.unknown_5d1d40();
                    serial_manager_00a40a28.Translate_Line_Ends();
                    serial_manager_00a40a28.Clear_Receive_Buffer();
                    WWMessageBox(L" ").Process(StringTable::Load_String("TXT_UNABLE_FIND_MODEM", 0, "D:\\Ra2\\NullDlg.CPP", 0x1AFC),
                        StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x1AFC), 0, 0, false);
                    serial_session_flags_00a3fac0 |= 1;
                    return connected;
                }
                break;
            default:
                WWMessageBox(L" ").Process(StringTable::Load_String("TXT_UNABLE_FIND_MODEM", 0, "D:\\Ra2\\NullDlg.CPP", 0x1B03),
                    StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x1B03), 0, 0, false);
                serial_session_flags_00a3fac0 |= 1;
                return connected;
        }
    } else if (modemstatus == -1) {
        serial_manager_00a40a28.unknown_5d1d40();
        serial_manager_00a40a28.Translate_Line_Ends();
        serial_manager_00a40a28.Clear_Receive_Buffer();
        WWMessageBox(L" ").Process(StringTable::Load_String("TXT_ERROR_IN_INITSTRING", 0, "D:\\Ra2\\NullDlg.CPP", 0x1B0D),
            StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x1B0D), 0, 0, false);
        serial_session_flags_00a3fac0 |= 1;
        return connected;
    }

    Debug_Printf("Ready to answer\n");
    dialstatus = serial_manager_00a40a28.Answer_Modem(reconnect);
    switch (dialstatus) {
        case DIAL_CONNECTED:
            Debug_Printf("Answer_Modem returned 'DIAL_CONNECTED'\n");
            connected = true;
            break;
        case DIAL_NO_CARRIER:
            Debug_Printf("Answer_Modem returned 'DIAL_NO_CARRIER'\n");
            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_NO_CARRIER", 0, "D:\\Ra2\\NullDlg.CPP", 0x1B1D),
                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x1B1D), 0, 0, false);
            connected = false;
            break;
        case DIAL_ERROR:
            Debug_Printf("Answer_Modem returned 'DIAL_ERROR'\n");
            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_NUMBER_INVALID", 0, "D:\\Ra2\\NullDlg.CPP", 0x1B23),
                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x1B23), 0, 0, false);
            connected = false;
            break;
        case DIAL_TIMEOUT:
            Debug_Printf("Answer_Modem returned 'DIAL_TIMEOUT'\n");
            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_ERROR_TIMEOUT", 0, "D:\\Ra2\\NullDlg.CPP", 0x1B29),
                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x1B29), 0, 0, false);
            connected = false;
            break;
        case DIAL_CANCELED:
            Debug_Printf("Answer_Modem returned 'DIAL_CANCELLED'\n");
            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_ANSWERING_CANCELED", 0, "D:\\Ra2\\NullDlg.CPP", 0x1B2F),
                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\NullDlg.CPP", 0x1B2F), 0, 0, false);
            connected = false;
            break;
    }

    serial_manager_00a40a28.unknown_5d1d40();
    serial_manager_00a40a28.Translate_Line_Ends();
    serial_manager_00a40a28.Clear_Receive_Buffer();
    serial_session_flags_00a3fac0 |= 1;
    return connected;
}
