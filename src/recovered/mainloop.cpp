// mainloop.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/mainloop.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#define UNKNOWN_SESSION_DECLARE_SLOT34_ARG
#include "shared/main_loop.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/win32_api.hpp"
#include "shared/win32_window.hpp"
#include "shared/gscreen.hpp"
#include "shared/map_view.hpp"
#include "shared/crt_string.hpp"
#include "shared/rules.hpp"
#include "shared/surface.hpp"
#include "shared/dsurface.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/palette.hpp"
#include "shared/connection.hpp"
#include "shared/owner_talk.hpp"
#include "shared/winsock_interface.hpp"
#include "shared/cd_timer.hpp"
#include "shared/system_timer.hpp"
#include "shared/queue.hpp"
#include "shared/debug_printf.hpp"
#include "shared/event.hpp"
#include "shared/house.hpp"
#include "shared/id_source.hpp"
#include "shared/session_state.hpp"
#include "shared/tool_tip.hpp"
#include "shared/msgloop.hpp"
#include "shared/theme.hpp"
#include "shared/sound_available.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/tactical.hpp"
#include "shared/target.hpp"
#include "shared/object.hpp"
#include "shared/layer.hpp"
#include "shared/unknown_list_832470.hpp"
#include "shared/fogged_object.hpp"
#include "shared/keyboard.hpp"
#include "shared/load_dlg.hpp"
#include "shared/techno.hpp"
#include "shared/command.hpp"
#include "shared/string_table.hpp"
#include "shared/win_stub.hpp"
#include "shared/house_type.hpp"
#include "shared/gadget.hpp"
#include "shared/text_label.hpp"
#include "shared/null_modem.hpp"
#include "shared/name_crc.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/mouse.hpp"
#include "shared/text_print.hpp"
#include "shared/color_scheme.hpp"
#include "shared/progress_screen.hpp"


// Header-static dynamic initializers of the unit at 0x0053F270: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0053F270-0x0053F451); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0053F290, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0053f270.cpp; no source of its own.


// Dynamic initializer (0x0053F2C0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0053f270.cpp; no source of its own.


// Dynamic initializer (0x0053F2E0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0053f270.cpp; no source of its own.


// Dynamic initializer (0x0053F300, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0053f270.cpp; no source of its own.


// Dynamic initializer (0x0053F320, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0053f270.cpp; no source of its own.


// Dynamic initializer (0x0053F340, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0053f270.cpp; no source of its own.


// Dynamic initializer (0x0053F360, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0053f270.cpp; no source of its own.


// Dynamic initializer (0x0053F3A0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0053f270.cpp; no source of its own.


// Dynamic initializer (0x0053F3D0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0053f270.cpp; no source of its own.


// Dynamic initializer (0x0053F400, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0053f270.cpp; no source of its own.


// Dynamic initializer (0x0053F430, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0053f270.cpp; no source of its own.


// Dynamic initializer (0x0053F440, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0053f270.cpp; no source of its own.


// bool unknown_53f460() (0x0053F460): returns the MainLoop flag at 0x00A6EB30.
// This unit declares UnknownSessionA3D29C slot 34 with its argument (shared/unknown_globals.hpp).

bool unknown_53f460()
{
    return unknown_main_loop_flag_a6eb30;
}


// void Set_Main_Loop_Flag_0053f470() (0x0053F470)

void Set_Main_Loop_Flag_0053f470()
{
    unknown_main_loop_flag_a6eb30 = true;
}


// void Clear_Main_Loop_Flag_0053f480() (0x0053F480)

void Clear_Main_Loop_Flag_0053f480()
{
    unknown_main_loop_flag_a6eb30 = false;
}


// void __fastcall Motion_Capture() -- 0x0053F490
// Purpose: with motion capture on, grabs the game window's client area (less
// the screen shake) into the next surface of a Rules MovieTime minute
// sequence; when the sequence is full, writes every frame as capNNNN.pcx.

extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);
int __fastcall Write_PCX_File(FileClass &file, Surface &pic, PaletteClass *palette);
extern bool Debug_MotionCapture;

void __fastcall Motion_Capture()
{
    if (Debug_MotionCapture) {
        static Surface **_array = 0;
        static int _sequence = 0;
        static int _seqsize = Rules->MovieTime * 900.0;

        if (_array == 0) {
            _array = new Surface *[_seqsize];
            memset(_array, '\0', _seqsize * sizeof(Surface *));
        }

        if (_array == 0) {
            Debug_MotionCapture = false;
        }

        RECT client_rect;
        GetClientRect(MainWindow, &client_rect);

        POINT screen_pt;
        screen_pt.x = client_rect.left;
        screen_pt.y = client_rect.top;
        if (!ClientToScreen(MainWindow, &screen_pt)) {
            return;
        }

        POINT screen_pt2;
        screen_pt2.x = client_rect.right;
        screen_pt2.y = client_rect.bottom;
        if (!ClientToScreen(MainWindow, &screen_pt2)) {
            return;
        }

        Rect rect(screen_pt.x, screen_pt.y, client_rect.right, client_rect.bottom);

        if (Map.ShakeX != 0 || Map.ShakeY != 0) {
            if (Map.ShakeX > 0) {
                rect.X += Map.ShakeX;
                rect.Width -= Map.ShakeX;
            } else if (Map.ShakeX < 0) {
                rect.Width += Map.ShakeX;
            }
            if (Map.ShakeY > 0) {
                rect.Y += Map.ShakeY;
                rect.Height -= Map.ShakeY;
            } else if (Map.ShakeY < 0) {
                rect.Height += Map.ShakeY;
            }
        }

        if (_sequence < _seqsize) {
            if (_array[_sequence] == 0) {
                _array[_sequence] = new BSurface(rect.Width, rect.Height, 2);
            }

            if (_array[_sequence] != 0) {
                _array[_sequence]->Copy_From(Rect(0, 0, rect.Width, rect.Height), *unknown_surface_839950, rect, false, true);
            }
            _sequence++;

        } else {
            Debug_MotionCapture = false;

            DSurface temp_page(rect.Width, rect.Height, false);

            for (int index = 0; index < _sequence; index++) {
                char filename[30];
                sprintf(filename, "cap%04d.pcx", index);
                CCFileClass file(filename);
                temp_page.Copy_From(*_array[index], false, true);
                Write_PCX_File(file, temp_page, &unknown_palette_837e10);
            }

            _sequence = 0;
        }
    }
}


// void __fastcall Unknown_0053F780() (0x0053F780; traces "Disconnect_Gracefully")
// In a game-mode 4 game with an active transport and the forwarding route: lets the
// flagged connection's owner signal the router and waits up to 480 ticks for the
// router to shut down, then sends NET_PORT_UNREACHABLE unless the session's slot 2
// says otherwise. Do_Win, Do_Lose and 0x0053F840 call it at the end of a game.

void __fastcall Unknown_0053F780()
{
    Debug_Printf("Disconnect_Gracefully\n");
    if (unknown_game_mode_a3d298 == 4 && PacketTransport != 0 && unknown_flag_a3fba8) {
        if (unknown_connection_a409c4 != 0) {
            unknown_OwnerTalkClass_00ab6d18.Unknown_0063FC90();
            CDTimerClass<SystemTimerClass> timer = 480;
            while (timer.Value() != 0) {
                Service_Game_IO_00481fb0();
                if (unknown_OwnerTalkClass_00ab6d18.Wait_For_Router_Shutdown(10, false)) {
                    break;
                }
            }
        }
        if (unknown_session_a3d29c == 0 || !unknown_session_a3d29c->unknown_slot2()) {
            Send_Port_Unreachable();
        }
    }
}


// bool Main_Loop_End_0053f840() (0x0053F840): the end of Westwood's Main_Loop.
// This unit declares UnknownSessionA3D29C slot 34 with its argument (shared/unknown_globals.hpp).

bool __fastcall Main_Loop_End_0053f840()
{
    if ((Frame & 0x3FF) == 0 && !Unknown_00491A10()) {
        OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 8));
    }
    if (player_wins_a35db1 || player_loses_a40c78 || player_restarts_866250 || player_aborts_a35db0) {
        unknown_a3fabc++;
        Pause_Game_0065c3d0();
        if (player_wins_a35db1 || player_loses_a40c78) {
            if (ToolTips != 0) {
                ToolTips->Enable(false);
            }
        }
        if (player_wins_a35db1 || player_loses_a40c78) {
            if (unknown_session_a3d29c != 0 && (unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4)) {
                unknown_session_a3d29c->unknown_slot34(player_wins_a35db1);
            }
        }
        if (player_wins_a35db1) {
            if (unknown_game_mode_a3d298 == 4 && !unknown_flag_a417c8) {
                if (unknown_b2a7ec != 0) {
                    unknown_flag_a3d8d1 = true;
                }
                Unknown_0069A160();
                Unknown_00698D00(false);
            }
            player_loses_a40c78 = false;
            player_wins_a35db1 = false;
            player_restarts_866250 = false;
            player_aborts_a35db0 = false;
            Unknown_Reload_Art_Set_006F31B0();
            unknown_session_a3d298.Unknown_00672E70();
            Do_Win_0065d7f0();
            unknown_session_a3d298.Unknown_00672DE0();
            unknown_main_loop_flag_a6eb30 = false;
            if (ToolTips != 0) {
                ToolTips->Enable(unknown_tooltips_enabled_a40b34);
            }
        } else if (player_loses_a40c78) {
            if (unknown_game_mode_a3d298 == 4 && !unknown_flag_a417c8) {
                if (unknown_b2a7ec != 0) {
                    unknown_flag_a3d8d1 = true;
                }
                Unknown_0069A160();
                Unknown_00698D00(false);
            }
            player_wins_a35db1 = false;
            player_loses_a40c78 = false;
            player_restarts_866250 = false;
            player_aborts_a35db0 = false;
            Unknown_Reload_Art_Set_006F31B0();
            unknown_session_a3d298.Unknown_00672E70();
            Do_Lose_0065dec0();
            unknown_session_a3d298.Unknown_00672DE0();
            unknown_main_loop_flag_a6eb30 = false;
            if (ToolTips != 0) {
                ToolTips->Enable(unknown_tooltips_enabled_a40b34);
            }
        } else if (player_restarts_866250) {
            player_wins_a35db1 = false;
            player_loses_a40c78 = false;
            player_restarts_866250 = false;
            player_aborts_a35db0 = false;
            unknown_session_a3d298.Unknown_00672E70();
            Do_Restart_0065e440();
            unknown_session_a3d298.Unknown_00672DE0();
            unknown_main_loop_flag_a6eb30 = false;
        } else if (player_aborts_a35db0) {
            player_wins_a35db1 = false;
            player_loses_a40c78 = false;
            player_restarts_866250 = false;
            player_aborts_a35db0 = false;
            Unknown_Reload_Art_Set_006F31B0();
            unknown_session_a3d298.Unknown_00672E70();
            Unknown_0065e590();
            unknown_session_a3d298.Unknown_00672DE0();
            unknown_main_loop_flag_a6eb30 = false;
            Unknown_0053F780();
        }
        unknown_a3fabc--;
    }
    return !unknown_flag_a40958;
}


// bool __fastcall unknown_53fbd0() (0x0053FBD0; Westwood's Main_Loop): one game frame; true when
// the game is over or a player flag ends it (0x0053F840 then runs the end-of-game handlers).
// This unit declares UnknownSessionA3D29C slot 34 with its argument (shared/unknown_globals.hpp).

extern "C" char *__cdecl strrchr(char const *string, int character);
extern "C" int __cdecl swprintf(wchar_t *buffer, wchar_t const *format, ...);

static inline void Do_Record_Playback()
{
    int count;
    int tgt;
    int i;
    Point2D coord;
    ObjectClass *obj;
    unsigned int sum;
    unsigned int sum2;
    unsigned int ltgt;

    if (unknown_session_a3d298.Record) {
        coord = unknown_tactical_83996c->Get_Tactical_Position();
        unknown_session_a3d298.RecordFile.Write(&coord, sizeof(coord));

        count = unknown_objects_a40c60.Count();
        unknown_session_a3d298.RecordFile.Write(&count, sizeof(count));

        sum = 0;
        for (i = 0; i < count; i++) {
            ltgt = TargetClass(unknown_objects_a40c60[i]).Encode();
            sum += ltgt;
        }
        unknown_session_a3d298.RecordFile.Write(&sum, sizeof(sum));

        for (i = 0; i < count; i++) {
            tgt = TargetClass(unknown_objects_a40c60[i]).Encode();
            unknown_session_a3d298.RecordFile.Write(&tgt, sizeof(tgt));
        }

        unknown_session_a3d298.RecordFile.Write(&unknown_team_event_a6ebc4, sizeof(unknown_team_event_a6ebc4));
        unknown_session_a3d298.RecordFile.Write(&unknown_team_number_a6ebc8, sizeof(unknown_team_number_a6ebc8));
        unknown_team_event_a6ebc4 = 0;
        unknown_team_number_a6ebc8 = 0;
    }

    if (unknown_session_a3d298.Play) {
        if (unknown_session_a3d298.RecordFile.Read(&coord, sizeof(coord)) == sizeof(coord)) {
            unknown_tactical_83996c->Set_Tactical_Position_006a6640(coord);
        }

        if (unknown_session_a3d298.RecordFile.Read(&count, sizeof(count)) == sizeof(count)) {
            sum = 0;
            for (i = 0; i < unknown_objects_a40c60.Count(); i++) {
                ltgt = TargetClass(unknown_objects_a40c60[i]).Encode();
                sum += ltgt;
            }

            unknown_session_a3d298.RecordFile.Read(&sum2, sizeof(sum2));
            if (sum2 != sum) {
                Unknown_00482BC0();
            }

            unknown_sound_enabled_7d88d6 = true;

            for (i = 0; i < count; i++) {
                if (unknown_session_a3d298.RecordFile.Read(&tgt, sizeof(tgt)) == sizeof(tgt)) {
                    TargetClass tmp;
                    tmp.Decode(tgt);
                    obj = tmp.As_Object();
                    if (obj && (sum2 != sum)) {
                        obj->unknown_slot80();
                        unknown_sound_enabled_7d88d6 = false;
                    }
                }
            }

            unknown_sound_enabled_7d88d6 = true;
        }

        unknown_session_a3d298.RecordFile.Read(&unknown_team_event_a6ebc4, sizeof(unknown_team_event_a6ebc4));
        unknown_session_a3d298.RecordFile.Read(&unknown_team_number_a6ebc8, sizeof(unknown_team_number_a6ebc8));

        Map.Flag_To_Redraw();
        Map.Render();
    }
}

bool __fastcall unknown_53fbd0()
{
    int input;
    int y;
    int x;

    if (!unknown_flag_a40958) {
        return !unknown_flag_a40958;
    }

    unknown_main_loop_flag_a6eb30 = true;

    while (!unknown_flag_a40d28) {
        if (unknown_session_a3d298.Type == 0 || unknown_session_a3d298.Type == 5) {
            Sleep(500);
            Windows_Message_Handler();
        } else {
            Sleep(10);
            Windows_Message_Handler();
            break;
        }
    }

    if (Frame >= unknown_session_a3d298.TrapFrame) {
        unknown_session_a3d298.Trap_Object_006715c0();
    }

    unknown_process_timer_a3d56c = timeGetTime();

    if (unknown_session_a3d298.TrapCheckHeap) {
        unknown_trap_check_heap_a40d1a = true;
    }

    if (unknown_flag_a40380 == true && unknown_check_406f30()) {
        int theme = Theme.Score;
        if (theme == -1) {
            theme = Theme.Pending;
        }
        if (unknown_flag_a40b51) {
            if (theme == -1) {
                Theme.Queue_Song(-2);
            }
        } else if (theme != -3) {
            Theme.Stop(true);
            Theme.Queue_Song(-3);
        }
    }

    if (unknown_session_a3d298.Type != 0 && unknown_session_a3d298.Type != 5 && unknown_session_a3d2ac == 2) {
        if (unknown_session_a3d298.Play) {
            unknown_frame_timer_839990 = 0;
        } else {
            if (unknown_a3d568 == 0) {
                unknown_frame_timer_839990 = 2;
                unknown_net_frame_timer_839970 = 33;
            } else {
                unknown_frame_timer_839990 = 60 / unknown_a3d568;
                unknown_net_frame_timer_839970 = 1000 / unknown_a3d568;
            }
            unknown_net_frame_delay_a6ebcc = unknown_net_frame_timer_839970;

            int maxahead = unknown_max_ahead_a3d560;
            int worst_latency = 0;
            if (unknown_session_a3d298.Type == 4) {
                if (maxahead > 30) {
                    for (int i = 0; i < unknown_ipx_manager_a40978.Num_Connections(); i++) {
                        if (worst_latency <= unknown_a3fb84[i]) {
                            worst_latency = unknown_a3fb84[i];
                        }
                    }
                    if (worst_latency) {
                        if (worst_latency >= maxahead / 4) {
                            unknown_net_frame_timer_839970 = unknown_net_frame_timer_839970 + 10;
                        }
                        if (worst_latency >= maxahead / 2) {
                            unknown_net_frame_timer_839970 = unknown_net_frame_timer_839970 + 10;
                        }
                        if (worst_latency >= (3 * maxahead) / 4) {
                            unknown_net_frame_timer_839970 = unknown_net_frame_timer_839970 + 10;
                        }
                    }
                }
            }
        }
    } else if (unknown_session_a3d298.Type == 0 && !unknown_flag_a40d84) {
        unknown_a40b18 = 2;
        unknown_frame_timer_839990 = 2;
    } else {
        unknown_frame_timer_839990 = unknown_a40b18;
    }

    if (unknown_session_a3d298.Type == 0) {
        if (unknown_id_source_a3d290->unknown_630 && unknown_id_source_a3d290->unknown_620.Value() <= 0) {
            unknown_id_source_a3d290->unknown_630 = false;
            Release_Pause_0065c310();
        }
        if (unknown_id_source_a3d290->unknown_62c) {
            Windows_Message_Handler();
            Service_Game_IO_00481fb0();
            unknown_tactical_83996c->AI();
            Map.Render();
            Sync_Delay();
            unknown_main_loop_flag_a6eb30 = false;
            return !unknown_flag_a40958;
        }
    }

    if (!unknown_session_a3d298.Play) {
        if (unknown_int_a40d48 == 0 && unknown_flag_a40d28) {
            Map.Get_Input(input, x, y);
            Keyboard_Process(input);
            if (unknown_flag_a3d8c4) {
                Unknown_005419E0(false);
            }
            if ((Frame & 7) == 7 && unknown_session_a3d298.Type == 4) {
                unknown_ipx_manager_a40978.Store_Stats();
            }
            FoggedObjectClass::unknown_4c1ae0();
            Map.Render();
        }
    }

    unknown_flag_a40d45 = false;

    if (unknown_session_a3d298.Record || unknown_session_a3d298.Play) {
        Do_Record_Playback();
    }

    unknown_layers_852578[2].Sort();

    if (unknown_session_a3d298.Type == 0 && !unknown_flag_a6ebd0) {
        wchar_t description[56];
        char name[261];
        unknown_flag_a6ebd0 = true;
        strcpy(name, unknown_id_source_a3d290->FileName);
        name[260] = '\0';
        char *dot = strrchr(name, '.');
        if (dot != 0) {
            *dot = '\0';
        }
        strcat(name, ".sav");
        swprintf(description, L"[%s]", unknown_id_source_a3d290->Name);
        Unknown_006554A0(name, description, 0);
    }

    unknown_list_832470.Unknown_0053D890();

    int scroll = unknown_scroll_rate_7dfb70;
    if (Main_Loop_Key_Down(KeyForceMove1) || Main_Loop_Key_Down(KeyForceMove2)) {
        scroll = scroll * unknown_scroll_factor_7dfb74;
    } else if (Main_Loop_Key_Down(KeyForceFire1) || Main_Loop_Key_Down(KeyForceFire2)) {
        scroll = unknown_int_8325d0 > unknown_int_8325d4 ? unknown_int_8325d0 : unknown_int_8325d4;
        scroll <<= 8;
    }
    if (unknown_scroll_keys_a6ebdc & 0x100) {
        Map.Scroll_Map(6, &scroll, true);
    }
    if (unknown_scroll_keys_a6ebdc & 0x1000) {
        Map.Scroll_Map(2, &scroll, true);
    }
    if (unknown_scroll_keys_a6ebdc & 0x01) {
        Map.Scroll_Map(0, &scroll, true);
    }
    if (unknown_scroll_keys_a6ebdc & 0x10) {
        Map.Scroll_Map(4, &scroll, true);
    }

    unknown_delete_all_a76648();
    unknown_session_a3d298.Messages.Update_Messages();

    unknown_process_ticks_a3d570 += min<int>(1000, timeGetTime() - unknown_process_timer_a3d56c);
    unknown_process_frames_a3d574++;

    Queue_AI();
    unknown_delete_all_a76648();
    Service_Game_IO_00481fb0();

    if (!player_wins_a35db1 && !player_loses_a40c78 && !player_restarts_866250 && !player_aborts_a35db0) {
        Frame++;
        if (unknown_ab9174 != 0 && unknown_ab9174 < Frame) {
            Resume_Game_0065c420();
            unknown_ab9174 = 0;
        }
        Sync_Delay();
        Unknown_006ecf10();
        Unknown_00614010();
        unknown_main_loop_flag_a6eb30 = false;
        return !unknown_flag_a40958;
    }

    unknown_main_loop_flag_a6eb30 = false;
    return true;
}


// void __fastcall Keyboard_Process(int &input) (0x005406E0; Red Alert's Keyboard_Process)
// Passes the key to Message_Input, then runs the hotkey command bound to it
// (modifier bits stripped first, then with them); without one, Escape queues the
// options event, Tab works the sidebar and the arrow keys set or clear the scroll bits.

void __fastcall Keyboard_Process(int &input)
{
    Message_Input(input);
    if (input == 0) {
        return;
    }
    unsigned int plain = input & ~0x800;
    unsigned int key = plain & 0xffffe8ff;
    CommandClass *command = unknown_hotkeys_832380[(int)key];
    if (command == 0) {
        command = unknown_hotkeys_832380[(int)plain];
    } else if (!command->unknown_slot5(plain)) {
        command = unknown_hotkeys_832380[(int)plain];
    }
    if (command != 0 && (!unknown_flag_a40d44 || _wcsicmp(command->Get_Category(), L"Debug") == 0)) {
        if (command->unknown_slot6(input)) {
            command->Execute(input);
        }
        if (command->unknown_slot7(input)) {
            int count = 0;
            int next = Main_Loop_Keyboard()->Check() & 0xffff;
            while (next == input) {
                Main_Loop_Keyboard()->Get();
                next = Main_Loop_Keyboard()->Check() & 0xffff;
                count++;
                if (count >= 10) {
                    break;
                }
            }
        }
        return;
    }
    if (key == 0x1B) {
        Queue_Options();
        return;
    }
    if (key == 9) {
        Sidebar_Tab_Key_006818b0(&Map);
        return;
    }
    if (key == 0x25) {
        if (input & 0x800) {
            unknown_scroll_keys_a6ebdc &= ~0x100;
        } else {
            unknown_scroll_keys_a6ebdc |= 0x100;
        }
        return;
    }
    if (key == 0x27) {
        if (input & 0x800) {
            unknown_scroll_keys_a6ebdc &= ~0x1000;
        } else {
            unknown_scroll_keys_a6ebdc |= 0x1000;
        }
        return;
    }
    if (key == 0x26) {
        if (input & 0x800) {
            unknown_scroll_keys_a6ebdc &= ~0x01;
        } else {
            unknown_scroll_keys_a6ebdc |= 0x01;
        }
        return;
    }
    if (key == 0x28) {
        if (input & 0x800) {
            unknown_scroll_keys_a6ebdc &= ~0x10;
        } else {
            unknown_scroll_keys_a6ebdc |= 0x10;
        }
    }
}


// void Sync_Delay() (0x00540960; Westwood's Sync_Delay)

void __fastcall Sync_Delay()
{
    unknown_spare_ticks_a4031c += unknown_frame_timer_839990.Value();
    if (unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5) {
        while (unknown_net_frame_timer_839970.Value()) {
            Service_Game_IO_00481fb0();
            if (unknown_int_a40d48 == 0 && unknown_flag_a40d28 == true) {
                int input = 0;
                int x;
                int y;
                if (unknown_net_frame_timer_839970.Value() > 10) {
                    Map.Get_Input(input, x, y);
                    Keyboard_Process(input);
                    unknown_tactical_83996c->AI();
                    Map.Render();
                } else {
                    Sleep(0);
                }
                if (!unknown_net_frame_timer_839970.Value()) {
                    break;
                }
            }
            Sleep(0);
        }
    } else {
        if (unknown_frame_timer_839990.Value()) {
            Wait_For_Surface_Blit_004961d0();
            while (unknown_frame_timer_839990.Value()) {
                Sleep(unknown_frame_timer_839990.Value());
            }
        }
    }

    static CDTimerClass<SystemTimerClass> fps_timer;
    if (!fps_timer) {
        unknown_a6eb1c = unknown_frames_this_second_a6eb18;
        unknown_frames_this_second_a6eb18 = 0;
        accumulated_fps_00a6eb20 += unknown_a6eb1c;
        fps_sample_count_00a6eb24++;
        if (accumulated_fps_00a6eb20 > 0x7FFFFFFF) {
            accumulated_fps_00a6eb20 = 0;
            fps_sample_count_00a6eb24 = 0;
        }
        fps_timer = 60;
    }
}


// atexit destructor (0x00540C10, _$E74) of Sync_Delay's static fps_timer (0x00A6EB60, guard bit
// 0x00A6EB81): CDTimerClass's empty destructor; emitted by the static in Sync_Delay_00540960.cpp,
// no source of its own.


// void __fastcall Message_Input(int &input) (0x00540C20; Westwood's Message_Input): feeds the
// IME result string to the message edit line; in a multiplayer game Return, Backspace or '\' opens
// the chat edit line (or the beacon text edit line while a beacon is being placed); passes the
// key to the edit line, mirrors it to the edited beacon, and on a send (3, or 4 with overflow)
// sends the text to the modem peer, as a WOL page or reply, or to the chosen players.


extern "C" wchar_t *__cdecl wcsncat(wchar_t *destination, wchar_t const *source, unsigned int count);
extern "C" int __cdecl wcsncmp(wchar_t const *a, wchar_t const *b, unsigned int count);
extern "C" int __cdecl wcscmp(wchar_t const *a, wchar_t const *b);
extern "C" char *__cdecl strcpy(char *destination, char const *source);
#pragma intrinsic(strcpy)
extern NullModemClass serial_manager_00a40a28;     // 0x00A40A28 (also shared/serial_progress.hpp)

void __fastcall Message_Input(int &input)
{
    int rc;
    int i;
    int count;
    wchar_t name[16];
    wchar_t command[64];
    wchar_t command2[64];
    wchar_t edit[162];
    wchar_t message[224];
    wchar_t prefix[112];
    wchar_t rest[224];
    wchar_t txt[224];

    int len = wcslen(IMEResultString);
    if (len) {
        int key = 0;
        for (int k = 0; k < len; k++) {
            unknown_session_a3d298.Messages.Input(key, IMEResultString[k]);
        }
        IMEResultString[0] = 0;
    }

    if (input == 0) {
        return;
    }

    if (unknown_session_a3d298.Type != 0 && unknown_session_a3d298.Type != 5 && !unknown_session_a3d298.Messages.IsEdit) {
        if (input & 0x8000) {
            return;
        }
        int ascii = (unsigned char)Main_Loop_To_ASCII(WWKeyboard, input);
        if (ascii == '\r' || ascii == '\\' || ascii == '\b') {
            txt[0] = 0;
            bool beacon = unknown_object_84e8d0.Unknown_0042E940(unknown_player_a35db4->ArrayIndex) != 0;
            if (beacon) {
                swprintf(txt, StringTable::Load_String("TXT_BEACON_MESSAGE", 0, "D:\\Ra2\\MainLoop.CPP", 1796));
            } else {
                swprintf(txt, StringTable::Load_String("TXT_FROM", 0, "D:\\Ra2\\MainLoop.CPP", 1798), unknown_player_a35db4->UIName);
                if (ascii == '\r') {
                    unknown_chat_target_a6ebe0 = 1;
                } else if (ascii == '\\') {
                    unknown_chat_target_a6ebe0 = 3;
                } else if (ascii == '\b') {
                    unknown_chat_target_a6ebe0 = 2;
                }
            }
            input = 0;
            int color = unknown_session_a3d298.Unknown_006715D0(unknown_a3d3b4);
            TextLabelClass *label = unknown_session_a3d298.Messages.Add_Edit(color, 0x4046, txt, 0, beacon ? 10000 : -1);
            if (beacon) {
                if (label) {
                    label->IsHidden = true;
                }
                unknown_session_a3d298.Messages.unknown_18 = false;
                wcscpy(name, Narrow_To_Wide_006fa180("_"));
                unknown_object_84e8d0.EditBeaconMessage_0042ec20(name, -1, -1, false);
            } else {
                unknown_session_a3d298.Messages.unknown_18 = true;
            }
            Map.Flag_To_Redraw(false);
            IMEResultString[0] = 0;
        }
    }

    int copy_input = input;
    rc = unknown_session_a3d298.Messages.Input(input, 0);

    if (rc == 1 && unknown_session_a3d298.Type != 0) {
        Map.Flag_To_Redraw(false);
    } else if (rc == 2 && unknown_session_a3d298.Type != 0) {
        if (copy_input == 0x1B) {
            Map.Flag_To_Redraw(2);
            unknown_chat_target_a6ebe0 = 0;
        } else {
            Map.Flag_To_Redraw(false);
        }
    }

    if ((unknown_object_84e8d0.Unknown_0042E940(unknown_player_a35db4->ArrayIndex) && copy_input && rc == 1) || rc == 2 || rc == 4 || IMEComposing) {
        if (copy_input == 0x1B) {
            unknown_object_84e8d0.EditBeaconMessage_0042ec20(0, -1, -1, true);
        } else {
            wcsncpy(edit, unknown_session_a3d298.Messages.Get_Edit_Buf(), 162);
            edit[161] = 0;
            wcsncat(edit, IMECompositionString, 162);
            edit[161] = 0;
            wcsncat(edit, Narrow_To_Wide_006fa180("_"), 162);
            edit[161] = 0;
            unknown_object_84e8d0.EditBeaconMessage_0042ec20(edit, -1, -1, false);
        }
    }

    if ((rc == 3 || rc == 4) && unknown_session_a3d298.Type != 0 && unknown_session_a3d298.Type != 5) {
        if (unknown_session_a3d298.Type == 2 || unknown_session_a3d298.Type == 1) {
            char *packet = serial_manager_00a40a28.BuildBuf;
            if (rc == 3 || rc == 4) {
                *(int *)packet = 0x68;
            }
            WideToCharString_006fa0f0(packet + 4, unknown_players_a3fa7c[0]->Name, -1);
            packet[0x18] = (char)unknown_a3d3b4;
            if (rc == 3) {
                wcscpy((wchar_t *)(packet + 0x19), unknown_session_a3d298.Messages.Get_Edit_Buf());
            } else if (rc == 4) {
                wcscpy((wchar_t *)(packet + 0x19), unknown_session_a3d298.Messages.OverflowBuf);
                unknown_session_a3d298.Messages.OverflowBuf[0] = 0;
            }
            serial_manager_00a40a28.Send_Message(serial_manager_00a40a28.BuildBuf, 0xFA, 1);
            if (rc == 3 || rc == 4) {
                wcscpy(unknown_last_message_a3f122, (wchar_t *)(packet + 0x19));
            }
        } else if (unknown_session_a3d298.Type == 3 || unknown_session_a3d298.Type == 4) {
            if (unknown_object_84e8d0.Unknown_0042E940(unknown_player_a35db4->ArrayIndex)) {
                unknown_object_84e8d0.EditBeaconMessage_0042ec20(unknown_session_a3d298.Messages.Get_Edit_Buf(), -1, -1, true);
            } else {
                if (unknown_session_a3d298.Type == 4) {
                    if (rc == 3) {
                        wcscpy(message, unknown_session_a3d298.Messages.Get_Edit_Buf());
                    } else if (rc == 4) {
                        wcscpy(message, unknown_session_a3d298.Messages.OverflowBuf);
                    }
                    int page = Parse_Page_Command_00746480(message);
                    if (page) {
                        if (page == 1) {
                        int n = 0;
                        wcscpy(command, StringTable::Load_String("TXT_PAGE_COMMAND", 0, "D:\\Ra2\\MainLoop.CPP", 1984));
                        wchar_t *p = &message[wcslen(command)];
                        while (*p != 0 && wcsncmp(p, L" ", 1) == 0) {
                            p++;
                        }
                        while (*p != 0 && wcsncmp(p, L" ", 1) != 0 && n <= 10) {
                            name[n++] = *p++;
                        }
                        name[n] = 0;
                        while (*p != 0 && wcsncmp(p, L" ", 1) == 0) {
                            p++;
                        }
                        if (wcslen(name) == 0) {
                            unknown_session_a3d298.Messages.Add_Message(0, -1,
                                StringTable::Load_String("TXT_NO_NAME_SPECIFIED", 0, "D:\\Ra2\\MainLoop.CPP", 2000),
                                unknown_session_a3d298.Unknown_006715D0(unknown_a3d3b4), 0x4046, (int)(Rules->MessageDelay * 900.0), false);
                        } else if (wcslen(p) == 0) {
                            unknown_session_a3d298.Messages.Add_Message(0, -1,
                                StringTable::Load_String("TXT_CANT_SEND_BLANK_PAGE", 0, "D:\\Ra2\\MainLoop.CPP", 2005),
                                unknown_session_a3d298.Unknown_006715D0(unknown_a3d3b4), 0x4046, (int)(Rules->MessageDelay * 900.0), false);
                        } else {
                            swprintf(command2, L"%s > %s", unknown_players_a3fa7c[0]->Name, name);
                            unknown_session_a3d298.Messages.Add_Message(command2, unknown_a3d3b4, p,
                                unknown_session_a3d298.Unknown_006715D0(unknown_a3d3b4), 0x4046, (int)(Rules->MessageDelay * 900.0), false);
                        }
                        if (rc == 4) {
                            wchar_t *buf = unknown_session_a3d298.Messages.Get_Edit_Buf();
                            if (buf) {
                                swprintf(prefix, StringTable::Load_String("TXT_FROM", 0, "D:\\Ra2\\MainLoop.CPP", 2024), unknown_player_a35db4->UIName);
                                swprintf(rest, L"%s%s %s", command, name, buf);
                                unknown_session_a3d298.Messages.Remove_Edit();
                                unknown_session_a3d298.Messages.Add_Edit(unknown_a3d3b4, 0x4046, prefix, 0, -1);
                                buf = unknown_session_a3d298.Messages.Get_Edit_Buf();
                                if (buf) {
                                    wcscat(buf, rest);
                                }
                            }
                        }
                        }
                        unknown_session_a3d298.Messages.OverflowBuf[0] = 0;
                        return;
                    }
                    if (Parse_Online_Command_00746240(message)) {
                        unknown_session_a3d298.Messages.OverflowBuf[0] = 0;
                        return;
                    }
                    int reply = Reply_To_Last_Pager_007466c0(message);
                    if (reply) {
                        if (reply == 1) {
                        bool shortform = false;
                        wcscpy(command, StringTable::Load_String("TXT_REPLY_COMMAND", 0, "D:\\Ra2\\MainLoop.CPP", 2054));
                        wcscpy(command2, StringTable::Load_String("TXT_REPLY_COMMAND_SHORT", 0, "D:\\Ra2\\MainLoop.CPP", 2055));
                        wchar_t *p;
                        if (wcsncmp(message, command, wcslen(command)) == 0) {
                            p = &message[wcslen(command)];
                        } else {
                            p = &message[wcslen(command2)];
                            shortform = true;
                        }
                        while (*p != 0 && wcsncmp(p, L" ", 1) == 0) {
                            p++;
                        }
                        swprintf(prefix, L"%s > %s", unknown_players_a3fa7c[0]->Name, Narrow_To_Wide_006fa180(unknown_last_pager_b2ac48));
                        if (wcslen(Narrow_To_Wide_006fa180(unknown_last_pager_b2ac48)) == 0) {
                            unknown_session_a3d298.Messages.Add_Message(0, unknown_a3d3b4,
                                StringTable::Load_String("TXT_NAME_NOT_SPECIFIED", 0, "D:\\Ra2\\MainLoop.CPP", 2069),
                                unknown_session_a3d298.Unknown_006715D0(unknown_a3d3b4), 0x4046, (int)(Rules->MessageDelay * 900.0), false);
                        } else if (wcslen(p) > 0) {
                            unknown_session_a3d298.Messages.Add_Message(prefix, unknown_a3d3b4, p,
                                unknown_session_a3d298.Unknown_006715D0(unknown_a3d3b4), 0x4046, (int)(Rules->MessageDelay * 900.0), false);
                        } else {
                            unknown_session_a3d298.Messages.Add_Message(0, unknown_a3d3b4,
                                StringTable::Load_String("TXT_CANT_SEND_BLANK_PAGE", 0, "D:\\Ra2\\MainLoop.CPP", 2082),
                                unknown_session_a3d298.Unknown_006715D0(unknown_a3d3b4), 0x4046, (int)(Rules->MessageDelay * 900.0), false);
                        }
                        if (rc == 4) {
                            wchar_t *buf = unknown_session_a3d298.Messages.Get_Edit_Buf();
                            if (buf) {
                                swprintf(edit, StringTable::Load_String("TXT_FROM", 0, "D:\\Ra2\\MainLoop.CPP", 2094), unknown_player_a35db4->UIName);
                                if (shortform) {
                                    swprintf(rest, L"%s %s", command2, buf);
                                } else {
                                    swprintf(rest, L"%s %s", command, buf);
                                }
                                unknown_session_a3d298.Messages.Remove_Edit();
                                unknown_session_a3d298.Messages.Add_Edit(unknown_a3d3b4, 0x4046, edit, 0, -1);
                                buf = unknown_session_a3d298.Messages.Get_Edit_Buf();
                                if (buf) {
                                    wcscat(buf, rest);
                                }
                            }
                        }
                        }
                        unknown_session_a3d298.Messages.OverflowBuf[0] = 0;
                        return;
                    }
                }

                if (rc == 3 || rc == 4) {
                    unknown_session_a3d298.GPacket.Command = 12;
                }
                strcpy(unknown_session_a3d298.GPacket.Name, Wide_To_Narrow_006fa120(unknown_players_a3fa7c[0]->Name));
                unknown_session_a3d298.GPacket.Message.unknown_110 = unknown_session_a3d298.unknown_11c;
                unknown_session_a3d298.GPacket.Message.unknown_114 = Unknown_005bba50(unknown_session_a3d298.HostName_2380);
                unknown_session_a3d298.GPacket.Message.unknown_2f = 0;
                if (rc == 3) {
                    wcscpy(unknown_session_a3d298.GPacket.Message.Buf, unknown_session_a3d298.Messages.Get_Edit_Buf());
                } else if (rc == 4) {
                    wcscpy(unknown_session_a3d298.GPacket.Message.Buf, unknown_session_a3d298.Messages.OverflowBuf);
                    unknown_session_a3d298.Messages.OverflowBuf[0] = 0;
                }

                count = Houses.Count();
                if (unknown_int_a3d548 && (unknown_chat_target_a6ebe0 == 1 || unknown_chat_target_a6ebe0 == 2)) {
                    for (i = 0; i < count; i++) {
                        HouseClass *house = Houses[i];
                        if (house && house->Type->Multiplay
                            && ((!unknown_flags_a3f118[i] && unknown_chat_target_a6ebe0 == 1)
                                || (!Houses[unknown_players_a3fa7c[0]->ID]->Is_Ally(house) && unknown_chat_target_a6ebe0 == 2))) {
                            int scheme = Gadget_Color_Scheme();
                            unknown_session_a3d298.Messages.Add_Message(0, 0,
                                StringTable::Load_String("TXT_OBSERVER_CANT_SEND", 0, "D:\\Ra2\\MainLoop.CPP", 2167),
                                scheme, 0x16, (int)(Rules->MessageDelay * 900.0), true);
                            unknown_chat_target_a6ebe0 = 0;
                            break;
                        }
                    }
                }

                for (i = 0; i < count; i++) {
                    HouseClass *house = Houses[i];
                    if (house && house->Type->Multiplay
                        && ((unknown_flags_a3f118[i] && unknown_chat_target_a6ebe0 == 1)
                            || (Houses[unknown_players_a3fa7c[0]->ID]->Is_Ally(house) && unknown_chat_target_a6ebe0 == 2)
                            || unknown_chat_target_a6ebe0 == 3)) {
                        int connections = unknown_ipx_manager_a40978.Num_Connections();
                        for (int j = 0; j < connections; j++) {
                            int id = unknown_ipx_manager_a40978.Connection_ID(j);
                            if (wcscmp(house->UIName, unknown_ipx_manager_a40978.Connection_Name(id)) == 0) {
                                IPXAddressClass *address = unknown_ipx_manager_a40978.Connection_Address(id);
                                if (unknown_flag_a3fba8) {
                                    unknown_session_a3d298.GPacket.Message.unknown_2f |= unknown_OwnerTalkClass_00ab6d18.Unknown_0063F4C0(id);
                                } else {
                                    unknown_ipx_manager_a40978.Send_Global_Message(&unknown_session_a3d298.GPacket, 0x1C7, 1, address, 0, 0);
                                    unknown_ipx_manager_a40978.Service();
                                }
                            }
                        }
                    }
                }

                if (unknown_flag_a3fba8 && unknown_session_a3d298.GPacket.Message.unknown_2f) {
                    Debug_Printf("Sending chat, mask = %d\n", (unsigned char)unknown_session_a3d298.GPacket.Message.unknown_2f);
                    unknown_ipx_manager_a40978.Send_Global_Message(&unknown_session_a3d298.GPacket, 0x1C7, 1, &unknown_players_a3fa7c[0]->Address, 0, 0);
                    unknown_ipx_manager_a40978.Service();
                }

                wchar_t *from = Narrow_To_Wide_006fa180(unknown_session_a3d298.GPacket.Name);
                unknown_session_a3d298.Messages.Add_Message(from, unknown_session_a3d298.GPacket.Message.unknown_110,
                    unknown_session_a3d298.GPacket.Message.Buf, unknown_session_a3d298.Unknown_006715D0(unknown_session_a3d298.GPacket.Message.unknown_110),
                    0x4046, (int)(Rules->MessageDelay * 900.0), false);
                wcscpy(unknown_last_message_a3f122, unknown_session_a3d298.GPacket.Message.Buf);
            }
        }
        Map.Flag_To_Redraw(2);
    }

    if (rc == 3) {
        unknown_chat_target_a6ebe0 = 0;
    }
}


// void __fastcall Unknown_005419E0(bool noframecheck) (0x005419E0; Westwood's Multiplayer_Debug_Print):
// prints the timing value at 0x007EC498 (milliseconds, as seconds) when set, then, with the debug
// display on in a network game, the frame, frame-rate, latency and processing figures across
// the screen and the connection manager's own lines (the argument is not read).

extern "C" int __cdecl swprintf(wchar_t *buffer, wchar_t const *format, ...);
extern bool unknown_debug_display_a3d8c5;               // 0x00A3D8C5 (MultiplayerDebugCommandClass toggles it)

void __fastcall Unknown_005419E0(bool noframecheck)
{
    if (unknown_time_7ec498 != -1) {
        double seconds = unknown_time_7ec498 * 0.001;
        Fancy_Text_Printf(L"%.2f", unknown_surface_839964, &Rect(0, 0, 500, 15), &Point2D(100, 0),
            ColorSchemes[Gadget_Color_Scheme()], 0x1F, 6, seconds, unknown_id_source_a3d290->FileName);
    }

    if (unknown_debug_display_a3d8c5 && unknown_session_a3d298.Type != 0 && unknown_session_a3d298.Type != 1 && unknown_session_a3d298.Type != 2) {

        Surface *surface = unknown_surface_839964;
        WWMouse->Hide_Mouse();

        surface->Fill_Rect(Rect(0, 300, 639, 230), 0);

        wchar_t buffer[256];

        swprintf(buffer, L"Frame : %d", Frame);
        Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(0, 302),
            Fetch_Scheme_By_Name("Grey", 1), 0, 0x19);

        swprintf(buffer, L"FPS : %d", unknown_a6eb1c);
        Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(0, 312),
            Fetch_Scheme_By_Name("Grey", 1), 0, 0x19);

        swprintf(buffer, L"MaxAhead : %d", unknown_max_ahead_a3d560);
        Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(0, 322),
            Fetch_Scheme_By_Name("Grey", 1), 0, 0x19);

        swprintf(buffer, L"Resp Time : %d ms", (int)(unknown_ipx_manager_a40978.Response_Time() * 1000) / 60);
        Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(0, 332),
            Fetch_Scheme_By_Name("Grey", 1), 0, 0x19);

        swprintf(buffer, L"Req fps : %d", unknown_a3d568);
        Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(0, 342),
            Fetch_Scheme_By_Name("Grey", 1), 0, 0x19);

        swprintf(buffer, L"Process : %d", unknown_session_a3d298.Players[0]->unknown_63);
        Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(0, 352),
            Fetch_Scheme_By_Name("Grey", 1), 0, 0x19);

        swprintf(buffer, L"Rout resp: %d", (unknown_a402dc * 1000) / 60);
        Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(0, 362),
            Fetch_Scheme_By_Name("Grey", 1), 0, 0x19);

        swprintf(buffer, L"NFT : %d", unknown_net_frame_timer_839970);
        Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(0, 372),
            Fetch_Scheme_By_Name("Grey", 1), 0, 0x19);

        swprintf(buffer, L"LNFT : %d", unknown_net_frame_delay_a6ebcc);
        Fancy_Text_Printf(buffer, surface, &surface->Get_Rect(), &Point2D(0, 382),
            Fetch_Scheme_By_Name("Grey", 1), 0, 0x19);

        unknown_ipx_manager_a40978.Multiplayer_Debug_Print_00526d90();

        WWMouse->Show_Mouse();
    }
}


// TargetClass TargetClass::Decode(int value) (0x00541E90)
// Inline member of shared/target.hpp (TibSun's xTargetClass::Decode), emitted as a COMDAT
// where a caller keeps it out of line: Main_Loop's game playback (0x0053FBD0), once the
// inline budget left inside its record/playback helper is spent.


// IndexClass<int, CommandClass *>::Is_Archive_Same (0x00541EC0): the hotkey index's
// out-of-line copy, emitted by Keyboard_Process in this unit.

template bool IndexClass<int, CommandClass *>::Is_Archive_Same(int const &id) const;


// IndexClass<int, CommandClass *>::Search_For_Node (0x00541EE0): the hotkey index's
// out-of-line copy, emitted by Keyboard_Process in this unit.

template IndexClass<int, CommandClass *>::NodeElement const *IndexClass<int, CommandClass *>::Search_For_Node(int const &id) const;
