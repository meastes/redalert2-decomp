// int __fastcall Select_Serial_Dialog() (0x005CDF00)
// The serial game dialog (Red Alert's Select_Serial_Dialog; statement shape from the TibSun
// reconstruction nulldlg.cpp): settings edits the session's default port, dial and answer open a
// modem link and run the modem host or guest dialog, null modem tests the cable (Test_Null_Modem,
// inlined) and runs the host or guest dialog by who won. Returns the game type (0: none).
// The unit's first include of shared/vector_class.hpp is here (through queue.hpp): Destroy_Null_Connection
// (0x005CEBA0) inlines DynamicVectorClass<NodeNameType *>::Delete(T const &).
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
// @ref ?serial_manager_00a40a28@@3VNullModemClass@@A=00a40a28
// @ref ?TickCount@@3V?$TTimerClass@VSystemTimerClass@@@@A=00839980
// @ref ??BSystemTimerClass@@QBEJXZ=0069a430
// @ref ?unknown_modem_aborted_a72ec4@@3_NA=00a72ec4
// @ref ??_C@_13ECNF@?$AA?5?$AA?$AA@=007d64f4
// @ref ??_C@_0L@NPIH@TXT_CANCEL?$AA@=007db8c8
// @ref ??_C@_06DN@TXT_OK?$AA@=007db8f4
// @ref ??_C@_0BD@MKPO@D?3?2Ra2?2NullDlg?4CPP?$AA@=007e74bc
// @ref ??_C@_0BE@JIOC@TXT_SELECT_SETTINGS?$AA@=007e74a8
// @ref ??_C@_0BG@OFPE@TXT_MODEM_OR_LOOPBACK?$AA@=007e7474
// @ref ??_C@_0BE@IBLE@TXT_WAITING_CONNECT?$AA@=007e748c
// @ref ??_C@_04NGMP@ATH?$AN?$AA@=007e74a0

// @ref __imp__SetCommBreak@4=0079a10c
// @ref __imp__ClearCommBreak@4=0079a108
// @ref __imp__EscapeCommFunction@8=0079a104
WIN32_IMPORT BOOL WINAPI SetCommBreak(HANDLE file);
WIN32_IMPORT BOOL WINAPI ClearCommBreak(HANDLE file);
#define CLRDTR 6

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
                            Unknown_006FA0F0(DialString, unknown_session_a3d298.CallWaitStrings[settings->CallWaitStringIndex], -1);
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
