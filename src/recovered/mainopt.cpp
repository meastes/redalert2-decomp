// mainopt.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/mainopt.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/main_opt.hpp"
#include "shared/theme.hpp"
#include "shared/event.hpp"
#include "shared/house.hpp"
#include "shared/techno.hpp"
#include "shared/display.hpp"
#include "shared/map_cell.hpp"
#include "shared/map_view.hpp"
#include "shared/session_state.hpp"
#define FIREWALL_SETTINGS_DECLARE_CLEAR
#include "shared/ipx_interface.hpp"
#include "shared/ccini_class.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/wol_chat.hpp"
#include "shared/crt_string.hpp"
#include "shared/debug_printf.hpp"
#include "shared/main_opt_ipx.hpp"
#include "shared/hicolor.hpp"
#include "shared/wwmouse.hpp"
#include "shared/dsurface.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/lobby.hpp"
#include "shared/win32_window.hpp"
#include "shared/movie_globals.hpp"
#include "shared/cd_timer.hpp"
#include "shared/system_timer.hpp"


// Header-static dynamic initializers of the unit at 0x00541F60: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00541F60-0x00542141); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00541F80, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00541f60.cpp; no source of its own.


// Dynamic initializer (0x00541FB0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00541f60.cpp; no source of its own.


// Dynamic initializer (0x00541FD0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00541f60.cpp; no source of its own.


// Dynamic initializer (0x00541FF0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00541f60.cpp; no source of its own.


// Dynamic initializer (0x00542010, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00541f60.cpp; no source of its own.


// Dynamic initializer (0x00542030, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00541f60.cpp; no source of its own.


// Dynamic initializer (0x00542050, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00541f60.cpp; no source of its own.


// Dynamic initializer (0x00542090, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00541f60.cpp; no source of its own.


// Dynamic initializer (0x005420C0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00541f60.cpp; no source of its own.


// Dynamic initializer (0x005420F0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00541f60.cpp; no source of its own.


// Dynamic initializer (0x00542120, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00541f60.cpp; no source of its own.


// Dynamic initializer (0x00542130, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00541f60.cpp; no source of its own.


// Dynamic initializer (0x00542150) of the Options copy at 0x00A6EC38 (a jump to OptionsClass::OptionsClass).

OptionsClass unknown_options_a6ec38;


// int __stdcall Try_Mode_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) (0x00542160)
// The "about to try mode" dialog: 0x497 loads the text into control 0x5DB; the buttons
// 0x5C1 and 0x6CA store 1 and 2 through the DWL_USER pointer.

int __stdcall Try_Mode_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int handled = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (handled != 0) {
        return handled;
    }
    int *result = (int *)GetWindowLongA(window, 8);
    switch (message) {
        case 0x497: {
            wchar_t const *text = StringTable::Load_String("TXT_ABOUT_TO_TRY_MODE", 0, "D:\\Ra2\\MainOpt.cpp", 0xA2);
            HWND item = GetDlgItem(window, 0x5DB);
            SendMessageA(item, 0x4B2, 0, (LPARAM)text);
            break;
        }
        case 0x111:
            switch ((unsigned short)wparam) {
                case 0x5C1:
                    *result = 1;
                    break;
                case 0x6CA:
                    *result = 2;
                    break;
            }
            break;
    }
    return 0;
}


// bool __fastcall Try_Mode_Dialog() (0x00542210)
// Shows the "about to try mode" dialog 0xD9 and waits for a button or the quit check;
// true when the first button (result 1) was pressed.

bool __fastcall Try_Mode_Dialog()
{
    int result = -1;
    HWND dialog = unknown_create_dialog_5ff610(0xD9, Try_Mode_Proc, 0);
    if (dialog) {
        SetWindowLongA(dialog, 8, (LONG)&result);
        unknown_dialog_5ff7c0(dialog);
        while (result < 0) {
            if (unknown_6000d0()) {
                break;
            }
            unknown_517370(false);
        }
        unknown_dialog_5ff6e0(dialog);
    }
    return result == 1;
}


// void __fastcall Main_Options_Apply(HWND window) (0x00542280)
// Reads the Options dialog's controls into Options: detail (0x52B), difficulty (0x50F), the unit action
// lines, show hidden and tool tip boxes (0x601, 0x604, 0x602), scroll rate (0x52A) and the score, sound and
// voice sliders (0x52F, 0x532, 0x536). The shell twin of GameControlsDialog::Apply_Settings (0x004CFA00).

void __fastcall Main_Options_Apply(HWND window)
{
    HWND control = GetDlgItem(window, 0x52B);
    if (control) {
        int detail = SendMessageA(control, 0x400, 0, 0) ? 2 : 0;
        if (Options.DetailLevel != detail) {
            Options.DetailLevel = detail;
            Map.Unknown_0049E570();
        }
    }
    control = GetDlgItem(window, 0x50F);
    if (control) {
        Options.Difficulty = SendMessageA(control, 0x400, 0, 0);
    }
    control = GetDlgItem(window, 0x601);
    if (control) {
        Options.UnitActionLines = SendMessageA(control, 0xF0, 0, 0) == 1;
        TechnoClass::unknown_6d79c0(Options.UnitActionLines);
    }
    control = GetDlgItem(window, 0x604);
    if (control) {
        Options.ShowHidden = SendMessageA(control, 0xF0, 0, 0) == 1;
    }
    control = GetDlgItem(window, 0x602);
    if (control) {
        Options.ToolTips = SendMessageA(control, 0xF0, 0, 0) == 1;
    }
    control = GetDlgItem(window, 0x52A);
    if (control) {
        Options.ScrollRate = 6 - SendMessageA(control, 0x400, 0, 0);
    }
    control = GetDlgItem(window, 0x52F);
    if (control) {
        Options.Set_Score_Volume((float)(SendMessageA(control, 0x400, 0, 0) * 0.1), false);
        if (Options.ScoreVolume == 0.0f) {
            Theme.Queue_Song(Theme.Score == -1 ? Theme.Pending : Theme.Score);
            Theme.Stop(false);
        }
    }
    control = GetDlgItem(window, 0x532);
    if (control) {
        Options.Set_Sound_Volume((float)(SendMessageA(control, 0x400, 0, 0) * 0.1), false);
    }
    control = GetDlgItem(window, 0x536);
    if (control) {
        Options.Set_Voice_Volume((float)(SendMessageA(control, 0x400, 0, 0) * 0.1), false);
    }
}


// void __fastcall Main_Options_Dialog() (0x00542460)
// Runs the shell Options dialog 0xD5 over and over: keeps a copy of Options, applies the controls when it
// closes, and for 0x5CD shows dialog 0xD7 (after reading the multiplayer settings), for 0x5CE the dialog
// of 0x005DA360; any other result ends it with the options saved. Plays with the game flag cleared.

void __fastcall Main_Options_Dialog()
{
    bool game_flag = unknown_flag_a40958;
    int result;
    unknown_flag_a40958 = false;
    for (;;) {
        unknown_options_a6ec38 = Options;
        result = -1;
        HWND dialog = unknown_create_dialog_5ff610(0xD5, Main_Options_Proc, 0);
        if (!dialog) {
            continue;
        }
        SetWindowLongA(dialog, 8, (LONG)&result);
        unknown_dialog_5ff7c0(dialog);
        while (result < 0) {
            if (unknown_6000d0() == true) {
                break;
            }
            unknown_517370(false);
        }
        Main_Options_Apply(dialog);
        unknown_dialog_5ff6e0(dialog);
        switch (result) {
            case 0x5CD: {
                int confirm = -1;
                unknown_session_a3d298.Read_MultiPlayer_Settings();
                dialog = unknown_create_dialog_5ff610(0xD7, Main_Options_Proc2, 0);
                if (dialog) {
                    SetWindowLongA(dialog, 8, (LONG)&confirm);
                    unknown_dialog_5ff7c0(dialog);
                    while (confirm < 0) {
                        if (unknown_6000d0() == true) {
                            break;
                        }
                        unknown_517370(false);
                    }
                    unknown_dialog_5ff6e0(dialog);
                }
                continue;
            }
            case 0x5CE:
                Options.Unknown_005DA360();
                continue;
            default:
                Options.Save_Settings();
                unknown_flag_a40958 = game_flag;
                return;
        }
    }
}


// Network card, socket, destination-network and firewall options dialog.

int __stdcall Main_Options_Proc2(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int handled = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (handled) return handled;
    int *result = (int *)GetWindowLongA(window, 8);
    char text[16];
    char address[64];
    switch (message) {
    case 0x497:
        {
            HWND cards = GetDlgItem(window, 0x5d4);
            bool temporary = false;
            if (PacketTransport == 0) {
                PacketTransport = new IPXInterfaceClass;
                PacketTransport->Init();
                temporary = true;
            }
            if (PacketTransport != 0) {
                Debug_Printf("Creating IPX Interface\n");
                IPXInterfaceClass *transport = (IPXInterfaceClass *)PacketTransport;
                for (int card = 0; card < 20; ++card) {
                    if (transport->unknown_76a230(card, address, 64)) {
                        Debug_Printf("Got address for network card %d\n", card);
                        Debug_Printf("Address is %s\n", address);
                        if (SendMessageA(cards, 0x4b8, 0, (LPARAM)address) == -1) {
                            SendMessageA(cards, 0x4bc, 0, (LPARAM)address);
                        }
                    }
                }
            }
            if (temporary) {
                delete PacketTransport;
                PacketTransport = 0;
            }
            if (SendMessageA(cards, 0x146, 0, 0) == 0) {
                SendMessageA(cards, 0x4c2, 0, (LPARAM)StringTable::Load_String("TXT_NO_BOUND_NET_CARD", 0, "D:\\Ra2\\MainOpt.cpp", 886));
            }
            int selected = Options.NetCard;
            if (selected < 0 || selected >= SendMessageA(cards, 0x146, 0, 0)) selected = 0;
            SendMessageA(cards, 0x14e, selected, 0);
            SendMessageA(window, 0x464, 0, 0);
            unknown_card_selected_7dfdb8 = -1;
            HWND socket = GetDlgItem(window, 0x5d1);
            if (Options.Socket != 0xffff) {
                sprintf(text, "%d", Options.Socket);
                SendMessageA(socket, 0x4b4, 0, (LPARAM)text);
            }
            SendMessageA(socket, 0xc5, 5, 0);
            HWND destination = GetDlgItem(window, 0x5d3);
            if (strlen(Options.DestNet)) SendMessageA(destination, 0x4b4, 0, (LPARAM)Options.DestNet);
            SendMessageA(destination, 0xc5, 11, 0);
            if (unknown_session_a3d298.SendDelay)
                SendDlgItemMessageA(window, 0x762, 0xf1, 1, 0);
            else
                SendDlgItemMessageA(window, 0x762, 0xf1, 0, 0);
            HWND port = GetDlgItem(window, 0x5d7);
            if (port != (HWND)-1 && unknown_session_a3d298.PortNumberOverride != 0) {
                sprintf(text, "%d", unknown_session_a3d298.PortNumberOverride);
                SendMessageA(port, 0x4b4, 0, (LPARAM)text);
            }
        }
        break;
    case 0x111:
        switch (wparam & 0xffff) {
        case 1:
            {
                HWND socket_control = GetDlgItem(window, 0x5d1);
                SendMessageA(socket_control, 0x4b5, 16, (LPARAM)text);
                unsigned short socket;
                if (strlen(text)) {
                    socket = (unsigned short)atoi(text);
                    if (strlen(text) > 5 || socket > 0x3fff) {
                        WWMessageBox((unsigned short const *)L" ").Process(StringTable::Load_String("TXT_INVALID_SOCKET",0,"D:\\Ra2\\MainOpt.cpp",975),
                                    StringTable::Load_String("TXT_OK",0,"D:\\Ra2\\MainOpt.cpp",975),0,0,false);
                        return 0;
                    }
                } else socket = 0xffff;
                char destination[32];
                char saved_destination[32];
                HWND destination_control = GetDlgItem(window, 0x5d3);
                SendMessageA(destination_control, 0x4b5, 32, (LPARAM)destination);
                strncpy(saved_destination, destination, 32);
                if (strlen(destination)) {
                    char *token = strtok(destination, ".");
                    int network[4] = {-1,-1,-1,-1};
                    if (token) {
                        int *part = network;
                        do {
                            sscanf(token, "%x", part);
                            ++part;
                            token = strtok(0, ".");
                        } while (token);
                    }
                    if ((network[0] & 255) != network[0] || (network[1] & 255) != network[1] ||
                        (network[2] & 255) != network[2] || (network[3] & 255) != network[3]) {
                        WWMessageBox((unsigned short const *)L" ").Process(StringTable::Load_String("TXT_DESTNET_ADDR_ERROR",0,"D:\\Ra2\\MainOpt.cpp",1009),
                                    StringTable::Load_String("TXT_OK",0,"D:\\Ra2\\MainOpt.cpp",1009),0,0,false);
                        return 0;
                    }
                }
                HWND card_control = GetDlgItem(window, 0x5d4);
                Options.NetCard = SendMessageA(card_control, 0x147, 0, 0);
                Options.Socket = socket;
                strcpy(Options.DestNet, saved_destination);
                HWND port_control = GetDlgItem(window, 0x5d7);
                char port_text[32];
                memset(port_text, 0, sizeof(port_text));
                SendMessageA(port_control, 0x4b5, 8, (LPARAM)port_text);
                strncpy(address, port_text, 32);
                int port = atoi(address);
                if (port > 65535) {
                    WWMessageBox((unsigned short const *)L" ").Process(StringTable::Load_String("TXT_PORT_NUMBER_ERROR",0,"D:\\Ra2\\MainOpt.cpp",1050),
                                StringTable::Load_String("TXT_OK",0,"D:\\Ra2\\MainOpt.cpp",1050),0,0,false);
                    return 0;
                }
                if (SendDlgItemMessageA(window, 0x762, 0xf0, 0, 0) == 1) {
                    if (!unknown_session_a3d298.SendDelay) {
                        WWMessageBox((unsigned short const *)L" ").Process(StringTable::Load_String("TXT_FIREWALL_SETTINGS_WARNING1",0,"D:\\Ra2\\MainOpt.cpp",1059),
                                    StringTable::Load_String("TXT_OK",0,"D:\\Ra2\\MainOpt.cpp",1059),0,0,false);
                    }
                    unknown_session_a3d298.SendDelay = true;
                } else unknown_session_a3d298.SendDelay = false;
                if (port > 0 && unknown_session_a3d298.PortNumberOverride == 0) {
                    WWMessageBox((unsigned short const *)L" ").Process(StringTable::Load_String("TXT_FIREWALL_SETTINGS_WARNING2",0,"D:\\Ra2\\MainOpt.cpp",1070),
                                StringTable::Load_String("TXT_OK",0,"D:\\Ra2\\MainOpt.cpp",1070),0,0,false);
                }
                unknown_session_a3d298.PortNumberOverride = port;
                unknown_firewall_a71b18.Clear_005b9e70();
                RawFileClass settings("RA2.INI");
                unknown_ini_839740.Put_Bool("MultiPlayer", "SendDelay", unknown_session_a3d298.SendDelay);
                unknown_ini_839740.Put_Int("MultiPlayer", "PortNumberOverride", unknown_session_a3d298.PortNumberOverride, 0);
                unknown_ini_839740.Save(settings, false);
            }
            // The OK path closes through the same result as Cancel.
        case 2:
            *result = 2;
            break;
        case 0x5d4:
            {
                HWND card_control = GetDlgItem(window, 0x5d4);
                unknown_card_selected_7dfdb8 = SendMessageA(card_control, 0x147, 0, 0);
            }
            break;
        }
        break;
    case 0x4db:
        Unknown_005def80(lparam, wparam != 0);
        return 0;
    }
    return 0;
}


// bool __fastcall Set_Display_Mode(int width, int height) (0x005434E0)
// Changes the screen mode: windowed it only resizes the visible rectangle; full screen it drops the mouse
// and the primary surface, sets the video mode (restoring both on failure), then deletes the off-screen
// surfaces, resizes the main window, reallocates the surfaces and rebuilds the remap tables, the mouse,
// the map view, the tabs and the open dialogs.

bool __fastcall Set_Display_Mode(int width, int height)
{
    Debug_Printf("About to set video mode\n");
    Point2D old_size(unknown_8523e0, unknown_8523e4);
    int red_right = Get_Red_Right();
    int red_left = Get_Red_Left();
    int green_right = Get_Green_Right();
    int green_left = Get_Green_Left();
    int blue_right = Get_Blue_Right();
    int blue_left = Get_Blue_Left();
    WWMouse->Hide_Mouse();
    Wait_For_Surface_Blit_004961d0();
    if (unknown_flag_851cb0 == true) {
        unknown_window_839640 = Rect(0, 0, width, height);
        unknown_8523e0 = width;
        unknown_8523e4 = height;
        Debug_Printf("VisibleRect: %dx%d\n", width, height);
    } else {
        if (WWMouse) {
            Debug_Printf("Removing mouse handler\n");
            delete WWMouse;
            WWMouse = 0;
        }
        if (unknown_surface_839950) {
            Debug_Printf("Deleting primary surface\n");
            delete unknown_surface_839950;
            unknown_surface_839950 = 0;
        }
        if (!Set_Video_Mode(MainWindow, width, height, 16)) {
            Debug_Printf("Set_Video_Mode failed.\n");
            Debug_Printf("Restoring primary surface\n");
            unknown_surface_839950 = DSurface::Create_Primary(0);
            Debug_Printf("Restoring mouse handler\n");
            WWMouse = new WWMouseClass(unknown_surface_839950, MainWindow);
            void const *shape = MixFileClass::Retrieve("MOUSE.SHP");
            if (shape) {
                Point2D hotspot = Point2D(0, 0);
                WWMouse->Set_Cursor(hotspot, (ShapeFileStruct const *)shape, 0);
            }
            Map.Set_Default_Mouse(0, false);
            Map.Override_Mouse_Shape(0, false);
            WWMouse->Show_Mouse();
            return false;
        }
        Debug_Printf("Mode apparently set OK\n");
        unknown_window_839640 = Rect(0, 0, width, height);
        Debug_Printf("VisibleRect: %dx%d\n", width, height);
    }
    if (unknown_surface_839958) {
        Debug_Printf("Deleting AlternateSurface\n");
        delete unknown_surface_839958;
        unknown_surface_839958 = 0;
    }
    if (unknown_surface_839954) {
        Debug_Printf("Deleting HiddenSurface\n");
        delete unknown_surface_839954;
        unknown_surface_839954 = 0;
    }
    if (unknown_surface_839944) {
        Debug_Printf("Deleting TileSurface\n");
        delete unknown_surface_839944;
        unknown_surface_839944 = 0;
    }
    if (unknown_surface_839948) {
        Debug_Printf("Deleting SidebarSurface\n");
        delete unknown_surface_839948;
        unknown_surface_839948 = 0;
    }
    if (unknown_surface_839964) {
        Debug_Printf("Deleting CompositeSurface\n");
        delete unknown_surface_839964;
        unknown_surface_839964 = 0;
    }
    if (unknown_flag_851cb0 == false) {
        Debug_Printf("Setting new window size to %dx%d\n", width, height);
        SetWindowPos(MainWindow, 0, 0, 0, width, height, 2);
        Debug_Printf("Creating primary surface\n");
        unknown_surface_839950 = DSurface::Create_Primary(0);
    } else {
        Debug_Printf("Setting new window size to %dx%d\n", width, height);
        RECT rect;
        SetRect(&rect, 0, 0, width, height);
        AdjustWindowRectEx(&rect, GetWindowLongA(MainWindow, -16), GetMenu(MainWindow) != 0, GetWindowLongA(MainWindow, -20));
        MoveWindow(MainWindow, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 1);
    }
    Rect tactical = Get_Title_Screen_Area_006f1f30();
    {
        Rect sidebar(0, 0, unknown_int_7add80, unknown_window_839640.Height);
        Rect tile(0, 0, tactical.Width, unknown_window_839640.Height);
        Rect composite(0, 0, tactical.Width, unknown_window_839640.Height);
        Allocate_Surfaces(&unknown_window_839640, &composite, &tile, &sidebar, false);
    }
    unknown_surface_83995c = unknown_surface_839954;
    Debug_Printf("Recalc color remap tables\n");
    Unknown_00486010(red_right, red_left, green_right, green_left, blue_right, blue_left);
    if (WWMouse == 0) {
        WWMouse = new WWMouseClass(unknown_surface_839950, MainWindow);
        void const *shape = MixFileClass::Retrieve("MOUSE.SHP");
        if (shape) {
            Point2D hotspot = Point2D(0, 0);
            WWMouse->Set_Cursor(hotspot, (ShapeFileStruct const *)shape, 0);
        }
        Map.Set_Default_Mouse(0, false);
        Map.Override_Mouse_Shape(0, false);
        WWMouse->Show_Mouse();
    } else {
        ((WWMouseClass *)WWMouse)->Calc_Confining_Rect();
    }
    Map.unknown_slot42(tactical);
    Unknown_Reset_Art_006F33B0(unknown_8523e0, unknown_8523e4);
    Map.Init_IO();
    Map.unknown_slot54(1);
    Map.Reposition_Sidebar();
    Map.Flag_To_Redraw(2);
    for (HWND dialog = Unknown_00735B10(); dialog != 0; dialog = (HWND)unknown_previous_key((int)dialog)) {
        Unknown_005FF380(dialog);
        Unknown_005E9F00(dialog, &old_size);
    }
    WWMouse->Show_Mouse();
    Debug_Printf("Mode change complete.\n");
    return true;
}


// bool __fastcall Test_Display_Mode(int width, int height) (0x00543A70)
// Switches to the display mode, shows the confirmation dialog 0xD9 for 600 ticks (restarted at 300
// after the dialog is told to cancel) and keeps the mode when it is answered with 1; otherwise
// switches back to the Options resolution.

bool __fastcall Test_Display_Mode(int width, int height)
{
    int result = -1;
    Debug_Printf("Testing display mode @ %dx%d\n", width, height);
    WWMouse->Hide_Mouse();
    unknown_surface_839954->Fill(0);
    Unknown_004E0C90(true, unknown_surface_839954, 0);
    if (!Set_Display_Mode(width, height)) {
        return false;
    }
    unknown_surface_839954->Fill(0);
    Unknown_004E0C90(true, unknown_surface_839954, 0);
    WWMouse->Show_Mouse();
    unknown_5159b0(0, true);
    HWND dialog = unknown_create_dialog_5ff610(0xD9, Confirm_Mode_Proc, 0);
    if (dialog) {
        SetWindowLongA(dialog, 8, (LONG)&result);
        unknown_dialog_5ff7c0(dialog);
        CDTimerClass<SystemTimerClass> timer(600);
        while (result < 0) {
            if (unknown_6000d0() == true) {
                break;
            }
            unknown_517370(false);
            if (timer <= 0) {
                PostMessageA(dialog, 0x111, 0x6CA, 0);
                timer = 300;
            }
        }
        unknown_dialog_5ff6e0(dialog);
        if (result != 1) {
            Debug_Printf("Resetting display mode @ %dx%d\n", Options.ScreenWidth, Options.ScreenHeight);
            Set_Display_Mode(Options.ScreenWidth, Options.ScreenHeight);
            unknown_surface_83995c = unknown_surface_839954;
            return false;
        }
    }
    Debug_Printf("Keeping display mode @ %dx%d\n", width, height);
    unknown_surface_83995c = unknown_surface_839954;
    return true;
}


// int __stdcall Confirm_Mode_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) (0x00543C40)
// The display mode confirmation dialog: 0x497 loads its text into control 0x5DB; the buttons
// 0x5C1 and 0x6CA store 1 and 2 through the DWL_USER pointer.

int __stdcall Confirm_Mode_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int handled = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (handled != 0) {
        return handled;
    }
    int *result = (int *)GetWindowLongA(window, 8);
    switch (message) {
        case 0x497: {
            wchar_t const *text = StringTable::Load_String("GUI:ConfirmModeChange", 0, "D:\\Ra2\\MainOpt.cpp", 0x5D5);
            HWND item = GetDlgItem(window, 0x5DB);
            SendMessageA(item, 0x4B2, 0, (LPARAM)text);
            break;
        }
        case 0x111:
            switch ((unsigned short)wparam) {
                case 0x5C1:
                    *result = 1;
                    break;
                case 0x6CA:
                    *result = 2;
                    break;
            }
            break;
    }
    return 0;
}
