// queue.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/queue.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x00622FB0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x00622FB0-0x00623196); the other
// drafts of this plan are emitted by these includes.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"


// void UnknownState_006231A0::UnknownState_006231a0() -- 0x006231A0
// Purpose: clears +0x2EC, +0x300, the two 5-word arrays at +0x304 and
// +0x318, +0x32C..+0x334, and sets +0x338 to 0x20000.
// Assumptions: owner and fields unidentified.

struct UnknownState_006231a0 {
    char unknown_000[0x2EC];
    int unknown_2ec;                        // +0x2EC
    char unknown_2f0[0x300 - 0x2F0];
    int unknown_300;                        // +0x300
    int unknown_304[5];                     // +0x304
    int unknown_318[5];                     // +0x318
    int unknown_32c;                        // +0x32C
    int unknown_330;                        // +0x330
    int unknown_334;                        // +0x334
    int unknown_338;                        // +0x338
    UnknownState_006231a0();
};

UnknownState_006231a0::UnknownState_006231a0()
{
    unknown_300 = 0;
    unknown_2ec = 0;
    for (int index = 0; index < 5; index++) {
        unknown_318[index] = 0;
        unknown_304[index] = 0;
    }
    unknown_330 = 0;
    unknown_32c = 0;
    unknown_334 = 0;
    unknown_338 = 0x20000;
}


// 0x006231f0: function not yet identified
//
// Body: 18 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_006231f0(int param_1)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006231f0 {

extern void  FUN_00781d3b(unsigned int allocation_address);

void __fastcall FUN_006231f0(int object_address)
{
    if (*(int *)(object_address + 0x334) != 0) {
        FUN_00781d3b((unsigned int)(*(int *)(object_address + 0x334)));
    }
    return;
}

}  // namespace seed_006231f0


// Queue.CPP's file statics (0x00623210-0x00623370 drafts) are declared with
// their constructors and destructors only for this unit.
#define RA2_QUEUE_STATICS


// Dynamic initializer (0x00622FE0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00622fb0.cpp; no source of its own.


// Dynamic initializer (0x00623000, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00622fb0.cpp; no source of its own.


// Dynamic initializer (0x00623020, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00622fb0.cpp; no source of its own.


// Dynamic initializer (0x00623040, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00622fb0.cpp; no source of its own.


// Dynamic initializer (0x00623060, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00622fb0.cpp; no source of its own.


// Dynamic initializer (0x00623080, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00622fb0.cpp; no source of its own.


// Dynamic initializer (0x006230C0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00622fb0.cpp; no source of its own.


// Dynamic initializer (0x006230F0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00622fb0.cpp; no source of its own.


// Dynamic initializer (0x00623120, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00622fb0.cpp; no source of its own.


// Dynamic initializer (0x00623150, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00622fb0.cpp; no source of its own.


// Dynamic initializer (0x00623160, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00622fb0.cpp; no source of its own.


// Dynamic initializer (0x00623180, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_00622fb0.cpp; no source of its own.


// FrameSyncStruct::FrameSyncStruct() (0x00623210): an empty frame-sync
// record (frame -1). Out of line; the array initializers 0x00623230 and
// 0x006232D0 expand it.
#include "shared/queue.hpp"

FrameSyncStruct::FrameSyncStruct() :
    Frame(-1),
    unknown_04(0),
    unknown_08(0),
    unknown_0c(0),
    unknown_10(0),
    unknown_14(0)
{
}


// Queue.CPP's frame-sync records 0x00AABCF0 (seven; dynamic initializer 0x00623230).
FrameSyncStruct unknown_timing_aabcf0[7];


// void __fastcall Unknown_00623260() (0x00623260): IPXManagerClass::unknown_527800 in an IPX game.

void __fastcall Unknown_00623260()
{
    if (unknown_game_mode_a3d298 == 4) {
        unknown_ipx_manager_a40978.unknown_527800();
    }
}


// void __fastcall Unknown_00623280() (0x00623280): IPXManagerClass::unknown_5277f0 in an IPX game.

void __fastcall Unknown_00623280()
{
    if (unknown_game_mode_a3d298 == 4) {
        unknown_ipx_manager_a40978.unknown_5277f0();
    }
}


// Queue.CPP's count-up timer 0x00A780C0 (dynamic initializer 0x006232A0, which
// registers the empty atexit destructor 0x006232C0).
UnknownElapsedTimer unknown_timer_a780c0;


// atexit destructor of unknown_timer_a780c0 (0x006232C0, _$E) emitted by unknown_timer_a780c0_init_006232a0.cpp; no source of its own.


// The other players' frame-sync records 0x00AABDD0 (seven; dynamic initializer 0x006232D0).
FrameSyncStruct unknown_their_timing_aabdd0[7];


// The 256 saved game states 0x00A780D8 (dynamic initializer 0x00623320, which
// registers the atexit destructor 0x00623370).
UnknownSyncState unknown_sync_states_a780d8[256];


// atexit destructor of unknown_sync_states_a780d8 (0x00623370, _$E) emitted by unknown_sync_states_a780d8_init_00623320.cpp; no source of its own.


// bool __fastcall Queue_Mission(TargetClass whom, int mission, TargetClass const &target, TargetClass const &destination) (0x006233A0): a MEGAMISSION event for the player.

bool __fastcall Queue_Mission(TargetClass whom, int mission, TargetClass const &target, TargetClass const &destination)
{
    if (!OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, whom, mission, target, destination, TargetClass()))) {
        return false;
    }
    return true;
}


// bool __fastcall Queue_Mission(TargetClass whom, int mission, TargetClass const &target, TargetClass const &destination, int speed, int maxspeed) (0x00623480): a MEGAMISSION_F event.

bool __fastcall Queue_Mission(TargetClass whom, int mission, TargetClass const &target, TargetClass const &destination, int speed, int maxspeed)
{
    if (!OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, whom, mission, target, destination, speed, maxspeed))) {
        return false;
    }
    return true;
}


// bool __fastcall Queue_Options() (0x00623550): an OPTIONS event unless the player is out.

bool __fastcall Queue_Options()
{
    if (unknown_player_a35db4->unknown_13f || unknown_player_a35db4->unknown_140 || unknown_player_a35db4->unknown_13e) {
        return false;
    }
    if (!OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 0x0C))) {
        return false;
    }
    return true;
}


// bool __fastcall Queue_Event_22() (0x00623600): a type 0x22 event unless the player is out (name hypothesis).

bool __fastcall Queue_Event_22()
{
    if (unknown_player_a35db4->unknown_13f || unknown_player_a35db4->unknown_140 || unknown_player_a35db4->unknown_13e) {
        return false;
    }
    if (!OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 0x22))) {
        return false;
    }
    return true;
}


// bool __fastcall Queue_Exit() (0x006236B0): EXIT, or 0x27 when the master leaves a network game of more than two players.
#include "shared/session_state.hpp"

bool __fastcall Queue_Exit()
{
    unsigned char type = 0x13;
    if (unknown_game_mode_a3d298 == 4 && unknown_flag_a3fba8 && unknown_session_a3d298.Players.Count() > 2 && unknown_session_a3d298.Am_I_Master(0)) {
        type = 0x27;
    }
    if (!OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, type))) {
        return false;
    }
    return true;
}


#include "shared/main_loop.hpp"
#include "shared/house_type.hpp"
// Queue.CPP normal/playback dispatch, target 00623770. Owner authored draft, fresh canonical context.


static void Queue_AI_Normal()
{
    while (OutList.Count) {
        OutList.First().IsExecuted = false;
        if (!DoList.Add(OutList.First())) {
            ;
        }
        OutList.Next();
    }
    if (unknown_flags_a3f600 & 1) {
        Queue_Record();
    }
    if (!Execute_DoList(1, unknown_player_a35db4->Type->House, 0, 0, 0)) {
        unknown_flag_a40958 = false;
        return;
    }
    Clean_DoList(0);
}

static void Queue_Playback()
{
    int numevents;
    EventClass event;
    int i;
    int ok;
    int max_houses;
    int base_house;
    int key;

    if (WWKeyboard->Check()) {
        key = WWKeyboard->Get();
        if (key == 0x1B || (unknown_flags_a3f600 & 4)) {
            unknown_flag_a40958 = false;
            return;
        }
    }
    if ((unknown_flags_a3f600 & 4) && ::Frame > 0 && (unknown_mouse_x_a76b70 != WWMouse->Get_Mouse_X() || unknown_mouse_y_a76c78 != WWMouse->Get_Mouse_Y())) {
        unknown_flag_a40958 = false;
        return;
    }
    unknown_mouse_x_a76b70 = WWMouse->Get_Mouse_X();
    unknown_mouse_y_a76c78 = WWMouse->Get_Mouse_Y();

    Compute_Game_CRC();
    CRC[::Frame & 0xFF] = GameCRC;

    if (::Frame >= unknown_trap_print_crc_a40314) {
        if (unknown_flag_ab62e8) {
            Unknown_0062DBA0(::Frame % 256, 0);
        } else {
            Print_CRCs(0);
        }
        Unknown_00691320();
        Emergency_Exit(0);
    }

    if (::Frame == 0 && unknown_game_mode_a3d298 != 0) {
        return;
    }

    if ((unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5) && unknown_session_a3d2ac == 2) {
        if (::Frame != (((::Frame + (unknown_frame_send_rate_a3d564 - 1)) / unknown_frame_send_rate_a3d564) * unknown_frame_send_rate_a3d564)) {
            return;
        }
    }

    ok = 1;
    if (unknown_record_file_a3f59c.Read(&numevents, sizeof(numevents)) == sizeof(numevents)) {
        for (i = 0; i < numevents; i++) {
            if (unknown_record_file_a3f59c.Read(&event, sizeof(EventClass)) == sizeof(EventClass)) {
                event.IsExecuted = 0;
                DoList.Add(event);
            } else {
                ok = 0;
                break;
            }
        }
    } else {
        ok = 0;
    }
    if (!ok) {
        unknown_flag_a40958 = false;
        return;
    }

    if (unknown_game_mode_a3d298 == 0) {
        max_houses = 1;
        base_house = unknown_player_a35db4->Type->House;
    } else {
        max_houses = unknown_max_players_a3d558;
        base_house = 0;
    }
    if (!Execute_DoList(max_houses, base_house, 0, 0, 0)) {
        unknown_flag_a40958 = false;
        return;
    }
    Clean_DoList(0);
}

void __fastcall Queue_AI()
{
    if (unknown_flags_a3f600 & 2) {
        Queue_Playback();
    } else {
        switch (unknown_game_mode_a3d298) {
        case 0:
        case 5:
            Queue_AI_Normal();
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            Queue_AI_Multiplayer();
            break;
        }
    }
}


#include "shared/null_modem.hpp"
#include "shared/mono_class.hpp"
#include "shared/string_table.hpp"
#include "shared/ww_message_box.hpp"
extern "C" void *__cdecl memset(void *, int, unsigned int);

struct QueueTimeouts { int Resend, Dialog, Sync, Load; };
extern QueueTimeouts unknown_queue_timeouts_7ec438[6];
extern int unknown_sync_counts_a76b74[8][8];
extern bool unknown_flag_a3d8d1, unknown_flag_abd500;

extern NullModemClass unknown_null_modem_a40a28;
extern unsigned short const unknown_caption_7d64f4[];
MonoClass *__fastcall Mono_Page_0059da50(int page);
void Mono_Window_Show_0059d9c0();
int __fastcall Wait_For_Players(int first_time, ConnManClass *net, int resend, int dialog,
    int timeout, char *buffer, int sent, FrameSyncStruct *their);

#define WestwoodMAX(a,b) (((a)>(b))?(a):(b))
#define WestwoodMIN(a,b) (((a)<(b))?(a):(b))

static void __fastcall Generate_Process_Time_Event_00623b00(ConnManClass *net)
{
    EventClass packet;

        unsigned long response=net->Response_Time();
        switch(unknown_a3fba4) {
        case 0: break;
        case 1: response+=response>>1; break;
        case 2: response*=2; break;
        case 3: response*=3; break;
        default: goto latency_done;
        }
        Debug_Printf("Response time = %d\n",response);
latency_done:
        unsigned long timeout=response*8+15;
        if (timeout<120) timeout=120;
        net->Set_Timing(response+10,-1,timeout,false);
        if (unknown_ab62dc) {
            Mono_Window_Show_0059d9c0();
            MonoClass *page=Mono_Page_0059da50(4);
            page->Set_Cursor(0,23);
            page->Printf("Processing Ticks:%03d Frames:%03d\n",unknown_process_ticks_a3d570,unknown_process_frames_a3d574);
            Unknown_0059DA00();
        }
        packet.Type=0x21;
        packet.Data.ProcessTime.AverageTicks=unknown_process_ticks_a3d570/unknown_process_frames_a3d574;
        packet.Frame=Frame;
        packet.House=unknown_player_a35db4->ArrayIndex;
        OutList.Add(packet);
        unknown_process_ticks_a3d570=0;
        unknown_process_frames_a3d574=0;
        if (unknown_game_mode_a3d298==4 && (Frame&2047)==0) net->Reset_Response_Time(false);
}

static void __fastcall Generate_Real_Timing_Event_00623b00(ConnManClass *net, int my_sent)
{
    EventClass packet;
    int i;
        Debug_Printf("Generate_Real_Timing_Event\n");
        if(unknown_session_a3d298.unknown_2e4!=0 || unknown_session_a3d298.unknown_2e8!=0) {
            Debug_Printf("Sending precalculated network timings on frame %d\n");
            packet.Type=0x20;
            packet.Data.Timing.DesiredFrameRate=unknown_a3d580;
            packet.Data.Timing.MaxAhead=unknown_session_a3d298.unknown_2e4;
            packet.Data.Timing.FrameSendRate=(unsigned int)unknown_session_a3d298.unknown_2e8>30U?10:5;
            packet.Frame=Frame;
            packet.House=unknown_player_a35db4->ArrayIndex;
            OutList.Add(packet);
            unknown_session_a3d298.unknown_2e4=0;unknown_session_a3d298.unknown_2e8=0;
        
            return;
        }
        if(my_sent<5)return;

                int worst=0;
                for (i=0;i<unknown_session_a3d298.Players.Count();i++) {
                    int ticks=unknown_session_a3d298.Players[i]->unknown_63;
                    if (ticks==-1) return;
                    if (ticks>worst) worst=ticks;
                }
                int desired;
                if (worst==0) desired=60;
                else desired=WestwoodMAX(1,1000/worst);
                int cap;
                switch(unknown_a40b18) {
                case 0:cap=60;break;
                case 1:cap=45;break;
                default:cap=60/unknown_a40b18;break;
                }
                desired=WestwoodMIN(desired,cap);
                unsigned long response;
                if(unknown_game_mode_a3d298==4 && unknown_session_a3d298.ViaPacketRouter) {
                    response=((IPXManagerClass*)net)->unknown_526c70();
                    unsigned long worst_response=0;
                    for(i=0;i<net->Num_Connections();i++) {
                        worst_response=WestwoodMAX(worst_response,(unsigned long)unknown_their_timing_aabdd0[i].unknown_10);
                    }
                    response+=worst_response;
                } else response=net->Response_Time();
                int connection_response=net->Response_Time();
                unsigned char send_rate=(unsigned char)unknown_frame_send_rate_a3d564;
                if(unknown_game_mode_a3d298==4)send_rate=desired>30?10:5;
                int base=0;
                if(response!=0) {
                    switch(unknown_a3fba4) {
                    case 0:break;
                    case 1:response+=response>>1;base=10;connection_response+=connection_response/2;break;
                    case 2:response*=2;connection_response*=2;base=20;break;
                    case 3:response*=3;connection_response*=3;base=30;break;
                    default:goto timing_latency_done;
                    }
                    Debug_Printf("Response time = %d\n",response);
                }
timing_latency_done:
                int ahead=(response*desired)/120+base;
                if(ahead>unknown_max_ahead_a3d560) ahead=WestwoodMIN(ahead,unknown_max_ahead_a3d560+send_rate);
                ahead=((ahead-1+send_rate)/send_rate)*send_rate;
                ahead=WestwoodMAX(ahead,(int)send_rate*3);
                int max_ahead=((send_rate+249)/send_rate)*send_rate;
                ahead=WestwoodMIN(ahead,max_ahead);
                packet.Type=0x20;
                packet.Data.Timing.DesiredFrameRate=desired;
                packet.Frame=Frame;
                packet.House=unknown_player_a35db4->ArrayIndex;
                packet.Data.Timing.MaxAhead=ahead+(unknown_id_source_a3d290->Special().FogOfWar?10:0);
                packet.Data.Timing.FrameSendRate=send_rate;
                OutList.Add(packet);
                int timeout=connection_response*8+15;
                if(timeout<120)timeout=120;
                net->Set_Timing(connection_response+10,-1,timeout,false);
            
}

static void __fastcall Stop_Game_00623b00(bool cancelled)
{
    unknown_a3d8c8=0;
    unknown_a3d8cc=0;
    unknown_flag_a40958=false;
    if(unknown_ab62dc)Unknown_0059DA00();
    if(unknown_game_mode_a3d298==4) {
        unknown_flag_abd500=true;
        if(!cancelled)unknown_flag_a3d8d1=true;
        unknown_flag_a3d8d2=true;
        Unknown_0069A160();
        Unknown_00698D00(false);
    }
}

static int __fastcall Process_Send_Period_00623b00(ConnManClass *net)
{
    if(Frame!=(int)(((Frame+(unknown_frame_send_rate_a3d564-1))/unknown_frame_send_rate_a3d564)*unknown_frame_send_rate_a3d564)) {
        net->Service();
        if(unknown_ab62dc)Unknown_0059DA00();
        return 0;
    }
    return 1;
}

static void __fastcall Queue_AI_Multiplayer()
{
    if (unknown_game_mode_a3d298 == 5) return;
    ConnManClass *net;
    char *buffer;
    int maximum;
    bool is_error;
    static CDTimerClass<FrameTimerClass> skip_crc;
    int i;
    int rc;
    if (unknown_game_mode_a3d298 >= 3 && unknown_game_mode_a3d298 <= 4) {
        buffer = unknown_meta_packet_a3f81a;
        maximum = unknown_meta_size_a3fa48;
        net = &unknown_ipx_manager_a40978;
    } else {
        buffer = unknown_null_modem_a40a28.BuildBuf;
        maximum = unknown_null_modem_a40a28.MaxLen - 14;
        net = &unknown_null_modem_a40a28;
    }
    Compute_Game_CRC();
    CRC[Frame & 255] = GameCRC;
    if (Frame == 0 || unknown_a3d8c8) {
        for (i = 0; i < 7; i++) {
            unknown_their_timing_aabdd0[i].Frame = -1;
            unknown_their_timing_aabdd0[i].unknown_04 = 0;
            unknown_their_timing_aabdd0[i].unknown_08 = 0;
            unknown_their_timing_aabdd0[i].unknown_0c = 0;
            unknown_their_timing_aabdd0[i].unknown_14 = 0;
            unknown_their_timing_aabdd0[i].unknown_10 = 20;
        }
        unknown_my_sent_aabe78 = 0;
        skip_crc = Frame + sizeof(CRC)/sizeof(CRC[0]);
        memset(CRC,0,sizeof(CRC));
        memset(unknown_megamission_frames_aabe94,0,sizeof(unknown_megamission_frames_aabe94));
        unknown_ab62ec=0; unknown_ab62f0=0; unknown_ab62f4=false;
        unknown_ab62f5=false; unknown_ab62e0=false;
        memset(unknown_megamission_counts_aabdb0,0,sizeof(unknown_megamission_counts_aabdb0));
        for(i=0;i<32;i+=4) for(int j=0;j<8;j++) *(int *)((char *)&unknown_sync_counts_a76b74[j][0]+i)=0;
        if (unknown_a3d8c8) unknown_crc_a40770=unknown_a3d8cc ? 0 : GameCRC;
        unknown_frame_syncs_ab62e4=0;
        unknown_timer_a780c0=UnknownElapsedTimer();
        Send_FrameSync(net,unknown_my_sent_aabe78);
        rc=Wait_For_Players(1,net,unknown_queue_timeouts_7ec438[unknown_game_mode_a3d298].Resend,
            unknown_queue_timeouts_7ec438[unknown_game_mode_a3d298].Dialog,
            unknown_queue_timeouts_7ec438[unknown_game_mode_a3d298].Load,buffer,unknown_my_sent_aabe78,unknown_their_timing_aabdd0);
        if (rc!=0) {
            if (unknown_game_mode_a3d298==4) Unknown_0069A160();
            is_error=true;
            unknown_counter_a3fabc++;
            if (rc==7) WWMessageBox(unknown_caption_7d64f4).Process(
                StringTable::Load_String("TXT_SYSTEM_NOT_RESPONDING",0,"D:\\Ra2\\Queue.CPP",0x40d),
                StringTable::Load_String("TXT_OK",0,"D:\\Ra2\\Queue.CPP",0x40d),0,0,false);
            else if (rc==2) WWMessageBox(unknown_caption_7d64f4).Process(
                StringTable::Load_String("TXT_SCENARIOS_DO_NOT_MATCH",0,"D:\\Ra2\\Queue.CPP",0x410),
                StringTable::Load_String("TXT_OK",0,"D:\\Ra2\\Queue.CPP",0x410),0,0,false);
            else if (rc==3) WWMessageBox(unknown_caption_7d64f4).Process(
                StringTable::Load_String("TXT_QUEUE_FULL",0,"D:\\Ra2\\Queue.CPP",0x413),
                StringTable::Load_String("TXT_OK",0,"D:\\Ra2\\Queue.CPP",0x413),0,0,false);
            else if (rc==8) is_error=false;
            else if (rc==9) {
                WWMessageBox(unknown_caption_7d64f4).Process(
                    StringTable::Load_String("TXT_DISCON_ABORT",0,"D:\\Ra2\\Queue.CPP",0x419),
                    StringTable::Load_String("TXT_OK",0,"D:\\Ra2\\Queue.CPP",0x419),0,0,false);
                is_error=false;
            }
            unknown_counter_a3fabc--;
            Stop_Game_00623b00(!is_error);
            return;
        }
        FrameSyncStruct *timing=unknown_their_timing_aabdd0;
        for(i=0;i<7;i++)unknown_a3fb84[i]=0;
        for(;(int)timing<(int)(unknown_their_timing_aabdd0+7);timing++)timing->Frame=0;
        net->Reset_Response_Time(true);
        if(unknown_session_a3d2ac==2)Process_Send_Period_00623b00(net);
        if (unknown_a3d8c8) { unknown_a3d8cc=0; unknown_a3d8c8=0; }
    } else if ((Frame & 127)==0) {
        Generate_Process_Time_Event_00623b00(net);
    }
    if (unknown_session_a3d298.Am_I_Master(0) &&
        (unknown_session_a3d298.unknown_2e4!=0 || unknown_session_a3d298.unknown_2e8!=0 || !(char)Frame)) {
        Generate_Real_Timing_Event_00623b00(net,unknown_my_sent_aabe78);
    }
    if(unknown_session_a3d2ac==2) {
        if(!Process_Send_Period_00623b00(net)) {
            if(unknown_ab62dc)Unknown_0059DA00();
            return;
        }
    }
    {
        int sent=(unsigned short)Send_Packets(net,buffer,maximum,unknown_max_ahead_a3d560,unknown_my_sent_aabe78);
        unknown_my_sent_aabe78+=sent;
        if(sent)Debug_Printf("Frame %d, my sent = %d\n",Frame,unknown_my_sent_aabe78);
    }
    if(Frame==0) {if(unknown_ab62dc)Unknown_0059DA00();return;}
    rc=Wait_For_Players(0,net,180,
        ((int)net->Response_Time()*3 > unknown_queue_timeouts_7ec438[unknown_game_mode_a3d298].Sync)
            ? (int)net->Response_Time()*3 : unknown_queue_timeouts_7ec438[unknown_game_mode_a3d298].Sync,
        unknown_queue_timeouts_7ec438[unknown_game_mode_a3d298].Load,buffer,unknown_my_sent_aabe78,unknown_their_timing_aabdd0);
    if(rc!=0) {
        Debug_Printf("Wait_For_Players returned %d\n",rc);
        if(unknown_game_mode_a3d298==4)Unknown_0069A160();
        is_error=true;unknown_counter_a3fabc++;
        if(rc==7)WWMessageBox(unknown_caption_7d64f4).Process(StringTable::Load_String("TXT_SYSTEM_NOT_RESPONDING",0,"D:\\Ra2\\Queue.CPP",0x4ab),StringTable::Load_String("TXT_OK",0,"D:\\Ra2\\Queue.CPP",0x4ab),0,0,false);
        else if(rc==2)WWMessageBox(unknown_caption_7d64f4).Process(StringTable::Load_String("TXT_SCENARIOS_DO_NOT_MATCH",0,"D:\\Ra2\\Queue.CPP",0x4af),StringTable::Load_String("TXT_OK",0,"D:\\Ra2\\Queue.CPP",0x4af),0,0,false);
        else if(rc==3)WWMessageBox(unknown_caption_7d64f4).Process(StringTable::Load_String("TXT_QUEUE_FULL",0,"D:\\Ra2\\Queue.CPP",0x4b3),StringTable::Load_String("TXT_OK",0,"D:\\Ra2\\Queue.CPP",0x4b3),0,0,false);
        else if(rc==8)is_error=false;
        else if(rc==9){WWMessageBox(unknown_caption_7d64f4).Process(StringTable::Load_String("TXT_DISCON_ABORT",0,"D:\\Ra2\\Queue.CPP",0x4bb),StringTable::Load_String("TXT_OK",0,"D:\\Ra2\\Queue.CPP",0x4bb),0,0,false);is_error=false;}
        unknown_counter_a3fabc--;
        Stop_Game_00623b00(!is_error);
        return;
    }
    if(unknown_session_a3d298.Record)Queue_Record();
    if(!Execute_DoList(unknown_max_players_a3d558,0,net,&skip_crc,unknown_their_timing_aabdd0)) {
        Debug_Printf("Failure executing DoList\n");
        if(unknown_game_mode_a3d298==4)Unknown_0069A160();
        Stop_Game_00623b00(false);
        return;
    }
    Clean_DoList(net);
    if(unknown_ab62dc)Unknown_0059DA00();
    return;

}


// References verified on the required target; timer thunk full16-byte proof is in timer-thunk-proof.json.


// Queue.CPP CRC skip timer atexit destructor, original00624840 (RET+15NOP).
// The compiler emits this real function from Queue_AI_Multiplayer's static
// CDTimerClass<FrameTimerClass>. Its selector is _$E79 in this complete unit.
// No handwritten stand-in body; source/semantic identity and1+15 extent stay fixed.


// void __fastcall Wait_For_End_Game() (0x00624850; name hypothesis from its traces).
#include "shared/display.hpp"
#include "shared/map_view.hpp"
#include "shared/tactical.hpp"
#include "shared/serial_progress.hpp"
#include "shared/cd_timer.hpp"

// Defined in this unit: the compiler then reads it with a 32-bit load and a mask.
unsigned short unknown_my_sent_aabe78;

void __fastcall Wait_For_End_Game()
{
    ConnManClass *net;
    char *multi_packet_buf;
    int multi_packet_max;
    int packetlen;
    int id;
    int input;
    int x;
    int y;
    int i;

    Debug_Printf("Waiting to exit game\n");
    if (unknown_session_a3d298.Type >= 3 && unknown_session_a3d298.Type <= 4) {
        multi_packet_buf = unknown_meta_packet_a3f81a;
        net = &unknown_ipx_manager_a40978;
        multi_packet_max = unknown_meta_size_a3fa48;
    } else {
        multi_packet_buf = serial_manager_00a40a28.BuildBuf;
        net = &serial_manager_00a40a28;
        multi_packet_max = serial_manager_00a40a28.MaxLen - 0x0E;
    }
    unknown_my_sent_aabe78 += Send_Packets(net, multi_packet_buf, multi_packet_max, unknown_max_ahead_a3d560, unknown_my_sent_aabe78);

    CDTimerClass<SystemTimerClass> framesync_timer = 15;
    CDTimerClass<SystemTimerClass> timeout_timer = 600;
    UnknownElapsedTimer elapsed;

    Debug_Printf("Entering wait loop\n");
    while (true) {
        WWKeyboard->Check();
        if (framesync_timer.Value() == 0) {
            framesync_timer = 15;
            Debug_Printf("Resending framesync\n");
            Send_FrameSync(net, unknown_my_sent_aabe78);
        }
        net->Service();
        if (timeout_timer.Value() == 0) {
            Debug_Printf("Timed out waiting for end game!\n");
            return;
        }
        int count = 1;
        while (true) {
            if (!net->Get_Private_Message(multi_packet_buf, &packetlen, &id)) {
                break;
            }
            WWKeyboard->Check();
            if (unknown_session_a3d298.Type == 1 || unknown_session_a3d298.Type == 2) {
                switch (Process_Serial_Packet(multi_packet_buf, 0)) {
                case RC_NORMAL:
                    break;
                case RC_PLAYER_READY:
                case RC_SCENARIO_MISMATCH:
                case RC_DOLIST_FULL:
                    return;
                case RC_SERIAL_PROCESSED:
                    net->Service();
                    framesync_timer = 15;
                    timeout_timer = 600;
                    break;
                case RC_PLAYER_LEFT:
                case RC_HUNG_UP:
                    return;
                default:
                    return;
                }
            } else {
                if (Process_Receive_Packet(net, multi_packet_buf, id, packetlen, unknown_their_timing_aabdd0, &elapsed) == RC_DOLIST_FULL) {
                    Debug_Printf("Dolist is full!\n");
                    return;
                }
            }
            net->Service();
            if (count++ >= 64) {
                break;
            }
        }
        unknown_481fb0();
        if (!unknown_int_a40d48) {
            Map.Get_Input(input, x, y);
            Keyboard_Process(input);
            unknown_tactical_83996c->AI();
            Map.Render();
        }

        int their_oldest_frame = ::Frame + 1000;
        int oldest_index = -1;
        for (i = 0; i < net->Num_Connections(); i++) {
            if (unknown_their_timing_aabdd0[i].Frame < their_oldest_frame) {
                their_oldest_frame = unknown_their_timing_aabdd0[i].Frame;
                oldest_index = i;
            }
        }
        if (their_oldest_frame + (int)unknown_frame_send_rate_a3d564 >= ::Frame) {
            Debug_Printf("their_oldest_frame = %d\n", their_oldest_frame);
            Debug_Printf("All players have finished - ready to proceed to score screen\n");
            Send_FrameSync(net, unknown_my_sent_aabe78);
            Send_FrameSync(net, unknown_my_sent_aabe78);
            Send_FrameSync(net, unknown_my_sent_aabe78);
            Send_FrameSync(net, unknown_my_sent_aabe78);
            return;
        }
        Debug_Printf("Waiting for player %d on frame %d\n", oldest_index, unknown_their_timing_aabdd0[oldest_index].Frame);
    }
}


// int Wait_For_Players (0x00624C10; Red Alert's Wait_For_Players): waits for the other players, with the
// reconnect dialog, the kick votes and the frame-sync stall bars.
// The exit path reads Session through its raw names (game mode, the debug flags at +0x62C and +0x62D): VC6's
// reload order at the loop head and the pixel channel order depend on how many static locations the body names.
#include "shared/unknown_player_entry.hpp"
#include "shared/wol_sink.hpp"
#include "shared/lobby.hpp"
#include "shared/dsurface.hpp"
#include "shared/hicolor.hpp"
#include "shared/ipx_conn.hpp"
#include "shared/crt_string.hpp"
#include "shared/gscreen.hpp"
#include "shared/id_source.hpp"

extern int unknown_reconnect_result_ab5ed0;     // 0x00AB5ED0
extern bool unknown_flag_ab5ed8;                // 0x00AB5ED8
extern bool unknown_flag_ab62fc;                // 0x00AB62FC
extern HWND unknown_reconnect_dialog_a780d4;    // 0x00A780D4
extern int unknown_seconds_ab6300;              // 0x00AB6300
extern long unknown_elapsed_aabe90;             // 0x00AABE90
extern unsigned short const unknown_blank_839d4c[]; // 0x00839D4C
extern unsigned short const unknown_crlf_7d64f4[];  // 0x007D64F4
extern void *unknown_instance_b26180;           // 0x00B26180
extern HWND unknown_parent_b263d8;              // 0x00B263D8
extern bool unknown_flag_a3d8c5;                // 0x00A3D8C5
extern int unknown_stall_counts_a3d5d8[][26];   // 0x00A3D5D8 (per connection: frame stalls, command stalls)
int __fastcall Wait_For_Players(int first_time, ConnManClass *net, int resend_delta, int dialog_time, int timeout,
    char *multi_packet_buf, int my_sent, FrameSyncStruct *their);

enum {
    MAX_EVENTS = 64,
};

#define QTEXT(label, line) StringTable::Load_String(label, 0, "D:\\Ra2\\Queue.CPP", line)

static inline void Close_Reconnect_Dialog()
{
    void *dialog = WS_Find_Dialog_00735940(0xEA);
    if (dialog) {
        if (unknown_flag_ab5ed8) {
            WWMouse->Capture_Mouse();
        }
        Unknown_007357E0(dialog, 0);
        unknown_flag_a40380 = unknown_flag_ab62fc;
        Map.Flag_To_Redraw(2);
        if (unknown_flag_a40380) {
            Map.Render();
        }
    }
}

static inline int Process_Reconnect_Dialog(ConnManClass *net, CDTimerClass<SystemTimerClass> *timeout_timer, FrameSyncStruct *their,
    int num_conn, int reconn, int fresh, UnknownElapsedTimer *timer)
{
    int new_time;
    int oldest_index;
    int i;
    wchar_t buf[512];

    unknown_elapsed_aabe90 = timer->Timer - timer->Started;
    for (i = 0; i < num_conn; i++) {
        unknown_timing_aabcf0[i] = their[i];
    }
    if (fresh) {
        unknown_flag_ab62fc = unknown_flag_a40380;
        unknown_flag_a40380 = false;
        unknown_reconnect_result_ab5ed0 = -1;
        unknown_reconnect_dialog_a780d4 = (HWND)unknown_create_dialog_735700(unknown_instance_b26180, 0xEA, unknown_parent_b263d8,
            (UnknownDialogProc)Unknown_00627350, 0);
        if (unknown_reconnect_dialog_a780d4) {
            SetWindowLongA(unknown_reconnect_dialog_a780d4, 8, (long)&unknown_reconnect_result_ab5ed0);
            unknown_show_dialog_5ff7e0(unknown_reconnect_dialog_a780d4, 0xEA);
            ShowWindow(unknown_reconnect_dialog_a780d4, 1);
            SendMessageA(unknown_reconnect_dialog_a780d4, 0x4A9, 0, 1);
            unknown_flag_ab5ed8 = WWMouse->Is_Captured();
            if (unknown_flag_ab5ed8) {
                WWMouse->Release_Mouse();
            }
            WWMouse->Show_Mouse();
        }
    }
    if (unknown_reconnect_result_ab5ed0 == 2) {
        return 1;
    }
    new_time = timeout_timer->Value() / 60;
    if (fresh || new_time != unknown_seconds_ab6300) {
        unknown_seconds_ab6300 = new_time;
        HWND item = GetDlgItem(unknown_reconnect_dialog_a780d4, 0x650);
        if (item) {
            swprintf(buf, QTEXT("TXT_TIME_ALLOWED", 3856), unknown_seconds_ab6300);
            SendMessageA(item, 0x4B2, 0, (LPARAM)buf);
        }
        if (!(unknown_seconds_ab6300 & 1)) {
            PostMessageA(unknown_reconnect_dialog_a780d4, 0xF, 0, 0);
        }
        if (fresh) {
            while (unknown_session_a3d298.GlobalPackets_8a0.Count()) {
                delete unknown_session_a3d298.GlobalPackets_8a0[0];
                unknown_session_a3d298.GlobalPackets_8a0.Delete(0);
            }
            memset(unknown_session_a3d298.KickVotes, 0, sizeof(unknown_session_a3d298.KickVotes));
            memset(unknown_session_a3d298.KickVoters, 0xFF, sizeof(unknown_session_a3d298.KickVoters));
            HWND list = GetDlgItem(unknown_reconnect_dialog_a780d4, 0x651);
            if (list) {
                if (reconn) {
                    int oldest = 0x7FFFFFFF;
                    oldest_index = 0;
                    for (i = 0; i < num_conn; i++) {
                        if (their[i].Frame < oldest) {
                            oldest = their[i].Frame;
                            oldest_index = i;
                        }
                    }
                    if (unknown_session_a3d298.Type == 3 || unknown_session_a3d298.Type == 4) {
                        swprintf(buf, QTEXT("TXT_RECONNECTING_TO", 3911),
                            unknown_ipx_manager_a40978.Connection_Name(unknown_ipx_manager_a40978.Connection_ID(oldest_index)));
                    } else {
                        swprintf(buf, QTEXT("TXT_RECONNECTING_TO", 3917), unknown_session_a3d298.Players[1]->Name);
                    }
                    _DrawMessage_005c2da0(-1, (unsigned short const *)buf, list);
                    _DrawMessage_005c2da0(-1, unknown_blank_839d4c, list);
                    if (unknown_session_a3d298.Type == 4) {
                        wcscpy(buf, QTEXT("TXT_RECONNECT_HELP3", 3931));
                        wcscat(buf, (wchar_t const *)unknown_crlf_7d64f4);
                        wcscat(buf, QTEXT("TXT_RECONNECT_HELP3B", 3933));
                        wcscat(buf, (wchar_t const *)unknown_crlf_7d64f4);
                        wcscat(buf, QTEXT("TXT_RECONNECT_HELP3C", 3935));
                        _DrawMessage_005c2da0(-1, (unsigned short const *)buf, list);
                        _DrawMessage_005c2da0(-1, unknown_blank_839d4c, list);
                    }
                    if (unknown_session_a3d298.Type == 4 || unknown_session_a3d298.Type == 3) {
                        wcscpy(buf, QTEXT("TXT_RECONNECT_HELP2", 3942));
                        wcscat(buf, (wchar_t const *)unknown_crlf_7d64f4);
                        if (unknown_session_a3d298.Type == 4) {
                            wcscat(buf, QTEXT("TXT_RECONNECT_HELP2B", 3946));
                        } else if (unknown_session_a3d298.Type == 3) {
                            wcscat(buf, QTEXT("TXT_RECONNECT_HELP4", 3948));
                        }
                        _DrawMessage_005c2da0(-1, (unsigned short const *)buf, list);
                        _DrawMessage_005c2da0(-1, unknown_blank_839d4c, list);
                    }
                    _DrawMessage_005c2da0(-1, unknown_blank_839d4c, list);
                    _DrawMessage_005c2da0(-1, (unsigned short const *)QTEXT("TXT_RECONNECT_HELP5", 3958), list);
                    _DrawMessage_005c2da0(-1, (unsigned short const *)QTEXT("TXT_RECONNECT_HELP1", 3963), list);
                    _DrawMessage_005c2da0(-1, unknown_blank_839d4c, list);
                } else {
                    swprintf(buf, QTEXT("TXT_WAITING_FOR_CONNECTIONS", 3966));
                    _DrawMessage_005c2da0(-1, (unsigned short const *)buf, list);
                }
            }
        }
    }
    if (unknown_session_a3d298.GlobalPackets_8a0.Count() > 0) {
        Unknown_00627210(unknown_session_a3d298.GlobalPackets_8a0[0]->PlayerInfo.unknown_33, unknown_session_a3d298.GlobalPackets_8a0[0]->PlayerInfo.unknown_2f);
        delete unknown_session_a3d298.GlobalPackets_8a0[0];
        unknown_session_a3d298.GlobalPackets_8a0.Delete(0);
    }
    for (i = 0; i < unknown_session_a3d298.Players.Count(); i++) {
        int votes = unknown_session_a3d298.KickVotes[unknown_session_a3d298.Players[i]->ID];
        if (votes >= unknown_session_a3d298.Players.Count() - 1) {
            Debug_Printf("Kicking player %s from the game due to %d votes\n", unknown_session_a3d298.Players[i], votes);
            if (i == 0) {
                Debug_Printf("Outvoted player is me!\n");
                for (i = 1; i < unknown_session_a3d298.Players.Count(); i++) {
                    Kick_Player_Now(net, net->Connection_Index(unknown_session_a3d298.Players[i]->ID), their, 0);
                }
                break;
            }
            Kick_Player_Now(net, net->Connection_Index(unknown_session_a3d298.Players[i]->ID), their, 1);
        }
    }
    return 0;
}

static inline int Can_Advance(ConnManClass *net, int max_ahead, FrameSyncStruct *their, int *frame_stall, int *count_stall)
{
    int their_oldest_frame;
    int count_ok;
    int i;

    *frame_stall = -1;
    if (unknown_session_a3d55c == 1) {
        return 1;
    }
    their_oldest_frame = Frame + 1000;
    for (i = 0; i < net->Num_Connections(); i++) {
        if (their[i].Frame < their_oldest_frame) {
            their_oldest_frame = their[i].Frame;
            *frame_stall = i;
        }
    }
    count_ok = 1;
    for (i = 0; i < net->Num_Connections(); i++) {
        if ((unsigned)their[i].unknown_08 < (unsigned)their[i].unknown_04) {
            count_ok = 0;
            break;
        }
    }
    if (count_ok) {
        if (Frame < their_oldest_frame + max_ahead) {
            return 1;
        }
        i = -1;
    }
    if (Frame < their_oldest_frame + max_ahead) {
        *frame_stall = -1;
    }
    *count_stall = i;
    return 0;
}

int __fastcall Wait_For_Players(int first_time, ConnManClass *net, int resend_delta, int dialog_time, int timeout,
    char *multi_packet_buf, int my_sent, FrameSyncStruct *their)
{
    int packetlen;
    int id;
    int num_ready;
    int loop_count;
    int messages_this_loop;
    int input;
    int x, y;
    int i;
    RetcodeType rc;
    bool stall_drawn = false;
    Surface *surface = unknown_surface_839950;
    CDTimerClass<SystemTimerClass> retry_timer;
    CDTimerClass<SystemTimerClass> dialog_timer;
    CDTimerClass<SystemTimerClass> timeout_timer;
    UnknownElapsedTimer timer;
    CDTimerClass<SystemTimerClass> progress_timer;
    int reconnect_dlg = 0;
    CDTimerClass<SystemTimerClass> unused_a = dialog_time * 2;
    CDTimerClass<SystemTimerClass> unused_b = dialog_time / 2;

    num_ready = 0;
    retry_timer = resend_delta;
    dialog_timer = dialog_time;
    timeout_timer = timeout;
    timer = UnknownElapsedTimer();
    progress_timer = 180;
    CDTimerClass<SystemTimerClass> keepalive_timer = 300;
    loop_count = 0;
    unsigned long now = timeGetTime();
    for (i = 0; i < net->Num_Connections(); i++) {
        their[i].unknown_14 = now;
    }

    while (true) {
        WWKeyboard->Check();
        if (retry_timer.Value() == 0) {
            retry_timer = resend_delta;
            Send_FrameSync(net, my_sent);
            if (unknown_session_a3d298.Type == 4 && unknown_flag_a3fba8) {
                unknown_ipx_manager_a40978.unknown_5277f0();
                Debug_Printf("Time to send IM_STILL_HERE packets\n");
                UnknownGlobalPacket packet;
                packet.Command = 0x27;
                strncpy(packet.Name, Wide_To_Narrow_006fa120((unsigned short const *)unknown_session_a3d298.Players[0]->Name), 20);
                *(int *)&packet.unknown_2f[0] = unknown_player_a35db4->ArrayIndex;
                IPXAddressClass address = unknown_forward_address_a3fbac;
                address.Set_Port(unknown_forward_port_a3fbc4);
                *(IPXAddressClass *)&packet.unknown_2f[4] = address;
                for (i = 1; i < unknown_session_a3d298.Players.Count(); i++) {
                    int id2 = unknown_session_a3d298.Players[i]->ID;
                    if (id2 != unknown_player_a35db4->ArrayIndex) {
                        unknown_ipx_manager_a40978.Send_Global_Message(&packet, 0x5B, 0,
                            &unknown_session_a3d298.Players[i]->Address, unknown_node_ids_a402ba[i], 0);
                    }
                }
                unknown_481fb0();
            }
        }
        if (keepalive_timer.Value() == 0 && !first_time) {
            keepalive_timer = 300;
        } else if (first_time) {
            if (progress_timer.Value() == 0) {
                if (unknown_session_a3d298.Type == 1 || unknown_session_a3d298.Type == 2 || unknown_flag_a3fba8) {
                    Debug_Printf("Resending progress message\n");
                    if (unknown_session_a3d298.Type == 4) {
                        unknown_ipx_manager_a40978.unknown_5277f0();
                    }
                    unknown_session_a3d298.Loading_Progress_00672160(unknown_id_source_a3d290->unknown_223b ? 200 : 100);
                    progress_timer = 180;
                }
            }
        }
        net->Service();

        if (dialog_timer.Value() == 0) {
            if (unknown_session_a3d298.Type == 4) {
                unknown_ipx_manager_a40978.unknown_5277f0();
            }
            int fresh = reconnect_dlg == 0;
            int reconn = first_time == 0;
            if (Process_Reconnect_Dialog(net, &timeout_timer, their, net->Num_Connections(), reconn, fresh, &timer)) {
                Unknown_007357E0(unknown_reconnect_dialog_a780d4, 0);
                if (unknown_flag_ab5ed8) {
                    WWMouse->Capture_Mouse();
                }
                unknown_flag_a40380 = unknown_flag_ab62fc;
                Map.Flag_To_Redraw(2);
                if (reconnect_dlg) {
                    Close_Reconnect_Dialog();
                }
                if (unknown_session_a3d298.Type == 4) {
                    unknown_ipx_manager_a40978.unknown_527800();
                }
                if (!unknown_ab62e0) {
                    Send_Port_Unreachable();
                    return RC_CANCEL;
                }
                Send_Port_Unreachable();
                return RC_CANCEL + 1;
            }
            reconnect_dlg = 1;
        }

        if (timeout_timer.Value() == 0) {
            if (first_time) {
                if (reconnect_dlg) {
                    Close_Reconnect_Dialog();
                }
                Send_Port_Unreachable();
                if (unknown_session_a3d298.Type == 4) {
                    unknown_ipx_manager_a40978.unknown_527800();
                }
                return RC_NOT_RESPONDING;
            }
            if (Handle_Timeout(net, their)) {
                Map.Flag_To_Redraw(2);
                Map.Render();
                retry_timer = resend_delta;
                dialog_timer = dialog_time;
                timeout_timer = timeout;
            } else {
                if (reconnect_dlg) {
                    Close_Reconnect_Dialog();
                }
                if (unknown_session_a3d298.Type == 4) {
                    unknown_ipx_manager_a40978.unknown_527800();
                }
                return RC_NOT_RESPONDING;
            }
        }

        messages_this_loop = 0;
        while (messages_this_loop++ < MAX_EVENTS && net->Get_Private_Message(multi_packet_buf, &packetlen, &id)) {
            WWKeyboard->Check();
            if (unknown_session_a3d298.Type == 1 || unknown_session_a3d298.Type == 2) {
                rc = Process_Serial_Packet(multi_packet_buf, first_time);
                if (rc == RC_SERIAL_PROCESSED) {
                    net->Service();
                    retry_timer = resend_delta;
                    dialog_timer = dialog_time;
                    timeout_timer = timeout;
                    continue;
                } else if (rc == RC_PLAYER_LEFT) {
                    if (first_time) {
                        num_ready++;
                    }
                    break;
                } else if (rc == RC_HUNG_UP) {
                    if (Handle_Timeout(net, their)) {
                        Map.Flag_To_Redraw(2);
                        Map.Render();
                        retry_timer = resend_delta;
                        dialog_timer = dialog_time;
                        timeout_timer = timeout;
                    } else {
                        if (reconnect_dlg) {
                            Close_Reconnect_Dialog();
                        }
                        if (unknown_session_a3d298.Type == 4) {
                            unknown_ipx_manager_a40978.unknown_527800();
                        }
                        Send_Port_Unreachable();
                        return RC_NOT_RESPONDING;
                    }
                } else if (rc != RC_NORMAL) {
                    break;
                }
            }
            rc = Process_Receive_Packet(net, multi_packet_buf, id, packetlen, their, &timer);
            if (rc == RC_PLAYER_READY) {
                num_ready++;
            } else if (rc == RC_SCENARIO_MISMATCH) {
                if (reconnect_dlg) {
                    Close_Reconnect_Dialog();
                }
                if (unknown_session_a3d298.Type == 4) {
                    unknown_ipx_manager_a40978.unknown_527800();
                }
                return RC_SCENARIO_MISMATCH;
            } else if (rc == RC_DOLIST_FULL) {
                if (reconnect_dlg) {
                    Close_Reconnect_Dialog();
                }
                if (unknown_session_a3d298.Type == 4) {
                    unknown_ipx_manager_a40978.unknown_527800();
                }
                Send_Port_Unreachable();
                return RC_DOLIST_FULL;
            }
            net->Service();
        }

        if (reconnect_dlg) {
            net->Num_Connections();
        }
        if (first_time) {
            if (num_ready >= net->Num_Connections()) {
                break;
            }
        } else {
            int frame_stall;
            int count_stall;
            if (Can_Advance(net, unknown_max_ahead_a3d560, their, &frame_stall, &count_stall)) {
                break;
            }
            if (unknown_flag_a3d8c4 && unknown_flag_a3d8c5 && loop_count > 0 && (!stall_drawn || frame_stall != -1 || count_stall != -1)) {
                Unknown_005419E0(true);
            } else if (stall_drawn) {
                goto service;
            }
            if (frame_stall != -1) {
                if (unknown_flag_a3d8c4 && unknown_flag_a3d8c5) {
                    surface->Fill_Rect(Rect((frame_stall * 5 + 5) * 20, 400, 35, 3), Build_Hicolor_Pixel(255, 255, 40));
                }
                unknown_stall_counts_a3d5d8[frame_stall][0]++;
            }
            if (count_stall != -1) {
                if (unknown_flag_a3d8c4 && unknown_flag_a3d8c5) {
                    surface->Fill_Rect(Rect(count_stall * 100 + 140, 400, 40, 3), Build_Hicolor_Pixel(255, 40, 40));
                }
                unknown_stall_counts_a3d5d8[frame_stall][1]++;
            }
            stall_drawn = true;
        }
service:
        unknown_481fb0();
        if (!first_time && !unknown_int_a40d48 && !reconnect_dlg) {
            Map.Get_Input(input, x, y);
            Keyboard_Process(input);
            unknown_tactical_83996c->AI();
            Map.Render();
        }
        loop_count++;
    }
    if (reconnect_dlg) {
        Close_Reconnect_Dialog();
        if (net->Num_Connections() > 0) {
            unknown_b2a630 = 0;
            unknown_b2aaf8 = 0;
        }
    }
    if (stall_drawn && unknown_session_a3d298.unknown_2ec[0x340] && unknown_session_a3d298.unknown_2ec[0x341]) {
        Rect rect(100, 400, 540, 4);
        surface->Fill_Rect(rect, 0);
    }
    if (unknown_game_mode_a3d298 == 4) {
        unknown_ipx_manager_a40978.unknown_527800();
    }
    return RC_NORMAL;
}


// int __fastcall Send_Packets(ConnManClass *net, char *multi_packet_buf, int multi_packet_max, int max_ahead,
// int my_sent) (0x006261A0; Red Alert's Send_Packets with Build_Send_Packet and Add_Uncompressed_Events inline).
#include "shared/crt_memory.hpp"

static int Build_Send_Packet(void *buf, int bufsize, int frame_delay, int num_cmds, int cap, int *added);
static int Add_Uncompressed_Events(void *buf, int bufsize, int frame_delay, int size, int cap);

int __fastcall Send_Packets(ConnManClass *net, char *multi_packet_buf, int multi_packet_max, int max_ahead, int my_sent)
{
    int cap;
    int ack_req;
    int packetlen;
    int added;
    int total = 0;

    cap = OutList.Count;
    if (cap > 0x2000 - DoList.Count) {
        cap = 0x2000 - DoList.Count;
    }
    int count = OutList.Count;

    while (true) {
        if (cap == 0 || OutList.Count == 0 || unknown_session_a3d55c == 1) {
            ack_req = 0;
        } else {
            ack_req = 1;
        }
        packetlen = Build_Send_Packet(multi_packet_buf, multi_packet_max, max_ahead, my_sent, cap, &added);
        cap -= added;
        total += added;
        if (added) {
            Debug_Printf("Send_Packets - created packet with %d commands on frame %d\n", added, ::Frame);
            ack_req = 1;
        }
        net->Send_Private_Message(multi_packet_buf, packetlen, ack_req, -1);
        unknown_frame_syncs_ab62e4++;
        net->Service();
        if (OutList.Count == 0 || OutList.Count > count || cap == 0) {
            break;
        }
    }
    if (total) {
        Debug_Printf("Send_Packets - added total of %d commands on frame %d\n", total, ::Frame);
    }
    return total;
}

static int Build_Send_Packet(void *buf, int bufsize, int frame_delay, int num_cmds, int cap, int *added)
{
    int size = 0;
    EventClass *finfo = (EventClass *)buf;

    finfo->Type = 0x1C;
    *added = 0;
    if (unknown_session_a3d2ac == 2) {
        finfo->Frame = ((::Frame + frame_delay + (unknown_frame_send_rate_a3d564 - 1)) / unknown_frame_send_rate_a3d564)
            * unknown_frame_send_rate_a3d564;
    } else {
        finfo->Frame = ::Frame + frame_delay;
    }
    finfo->House = unknown_player_a35db4->ArrayIndex;
    finfo->LongAt(0x07) = GameCRC;
    finfo->ShortAt(0x0B) = num_cmds;
    finfo->ByteAt(0x0D) = frame_delay;
    if (unknown_session_a3d2ac == 0) {
        size += sizeof(EventClass);
    } else {
        size += 0x0E;
    }
    switch (unknown_session_a3d2ac) {
    case 0:
        size = Add_Uncompressed_Events(buf, bufsize, frame_delay, size, cap);
        break;
    case 1:
    case 2:
        size = Add_Compressed_Events(buf, bufsize, frame_delay, size, cap, added);
        break;
    default:
        size = 0;
        break;
    }
    return size;
}

static int Add_Uncompressed_Events(void *buf, int bufsize, int frame_delay, int size, int cap)
{
    int num = 0;
    int ev_size;

    while (OutList.Count && num < cap) {
        if (OutList.First().Type == 0x1F) {
            ev_size = sizeof(EventClass) + OutList.First().LongAt(0x0B);
        } else {
            ev_size = sizeof(EventClass);
        }
        if (size + ev_size > bufsize) {
            return size;
        }
        OutList.First().Frame = ::Frame + frame_delay;
        OutList.First().House = unknown_player_a35db4->ArrayIndex;
        OutList.First().IsExecuted = 0;
        if (!DoList.Add(OutList.First())) {
            return size;
        }
        if (OutList.First().Type == 0x1F) {
            memcpy((char *)buf + size, &OutList.First(), sizeof(EventClass));
            size += sizeof(EventClass);
            memcpy((char *)buf + size, (void *)OutList.First().LongAt(0x07), OutList.First().LongAt(0x0B));
            size += OutList.First().LongAt(0x0B);
        } else {
            memcpy((char *)buf + size, &OutList.First(), sizeof(EventClass));
            size += sizeof(EventClass);
        }
        num++;
        OutList.Next();
    }
    return size;
}


// void __fastcall Send_FrameSync(ConnManClass *net, int cmd_count) (0x00626500): a FRAMESYNC packet to every player.

void __fastcall Send_FrameSync(ConnManClass *net, int cmd_count)
{
    EventClass packet;

    memset(&packet, 0, sizeof(EventClass));
    packet.Type = 0x19;
    if (unknown_session_a3d2ac == 2) {
        packet.Frame = ((::Frame + unknown_max_ahead_a3d560 + (unknown_frame_send_rate_a3d564 - 1)) / unknown_frame_send_rate_a3d564) * unknown_frame_send_rate_a3d564;
    } else {
        packet.Frame = ::Frame + unknown_max_ahead_a3d560;
    }
    packet.House = unknown_player_a35db4->ArrayIndex;
    packet.LongAt(0x07) = unknown_crc_a40770;
    packet.ShortAt(0x0b) = cmd_count;
    packet.ByteAt(0x0d) = unknown_max_ahead_a3d560;
    net->Send_Private_Message(&packet, 0x0E, 0, -1);
    Debug_Printf("Sent FrameSync only packet\n");
    unknown_frame_syncs_ab62e4++;
}


// RetcodeType __fastcall Process_Receive_Packet(ConnManClass *net, char *multi_packet_buf, int id, int packetlen,
// FrameSyncStruct *their, UnknownElapsedTimer *timer) (0x006265C0; Red Alert's Process_Receive_Packet,
// with Breakup_Receive_Packet and Extract_Uncompressed_Events inline and the router's packets first).

static int Breakup_Receive_Packet(void *buf, int bufsize);
static int Extract_Uncompressed_Events(void *buf, int bufsize);

RetcodeType __fastcall Process_Receive_Packet(ConnManClass *net, char *multi_packet_buf, int id, int packetlen,
    FrameSyncStruct *their, UnknownElapsedTimer *timer)
{
    EventClass *event = (EventClass *)multi_packet_buf;
    RetcodeType retcode = RC_NORMAL;
    int index = net->Connection_Index(id);
    int i;

    if (unknown_flag_a3fba8 && (event->Type == 0x25 || event->Type == 0x26)) {
        int count = (char)event->House;
        unsigned long most = 0;
        if (event->Type == 0x26) {
            Debug_Printf("Received PACKETTIMING packet\n");
        } else if (event->Type == 0x25) {
            Debug_Printf("Received MEGAFRAMEINFO packet\n");
        }
        if (count == 0) {
            Debug_Printf("MEGAFRAMEINFO is part of a pserver port blitz!\n");
        }
        i = 0;
        if (i < count) {
            UnknownPacketTimingEntry *timing = (UnknownPacketTimingEntry *)&event->Data;
            UnknownMegaFrameEntry *mega = (UnknownMegaFrameEntry *)&event->Data;
            for (; i < count; i++, mega++, timing++) {
                if (event->Type == 0x25) {
                    int player = net->Connection_Index(mega->ID);
                    if (player == -1) {
                        continue;
                    }
                    if (their[player].Frame < (long)(mega->Frame - mega->Delay)) {
                        their[player].Frame = mega->Frame - mega->Delay;
                        Unknown_0062E5C0(their, player);
                        if (unknown_ipx_manager_a40978.Connection_Name(mega->ID)) {
                            Debug_Printf("Got MEGAFRAMEINFO frame update from %s. New frame = %d\n",
                                Wide_To_Narrow_006fa120(unknown_ipx_manager_a40978.Connection_Name(mega->ID)), their[player].Frame);
                        }
                        if ((unsigned)their[player].unknown_04 < mega->CommandCount) {
                            their[player].unknown_04 = mega->CommandCount;
                            Debug_Printf("Command count is %d\n", their[player].unknown_04);
                        }
                    } else {
                        if (unknown_ipx_manager_a40978.Connection_Name(mega->ID)) {
                            Debug_Printf("Got useless MEGAFRAMEINFO frame update from %s. frame = %d\n",
                                Wide_To_Narrow_006fa120(unknown_ipx_manager_a40978.Connection_Name(mega->ID)), their[player].Frame);
                        }
                    }
                    if (!(mega->Flags & 1)) {
                        their[player].unknown_0c = timer->Timer - timer->Started;
                    }
                } else {
                    if (timing->ID == unknown_player_a35db4->ArrayIndex) {
                        unsigned long response = timing->ResponseTime;
                        unknown_a402dc = response;
                        Debug_Printf("My router response time = %d ms\n", response * 1000 / 60);
                        continue;
                    }
                    int player = net->Connection_Index(timing->ID);
                    if (player == -1) {
                        continue;
                    }
                    unsigned long response = timing->ResponseTime;
                    their[player].unknown_10 = response;
                    unknown_response_times_a76c88[player] = response;
                    if (most <= response) {
                        most = response;
                    }
                    if (timing->unknown_01 && unknown_session_a3d298.Am_I_Master(0) && unknown_a402d4 != timing->ID) {
                        OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 0x28, timing->ID));
                    }
                }
            }
        }
        if (count && most) {
            unknown_a402cc = most;
        }
    } else {
    FrameSyncStruct *t = &their[index];
    if (t->Frame < (long)(event->Frame - event->ByteAt(0x0D))) {
        if (t->Frame == -1) {
            retcode = RC_PLAYER_READY;
            Debug_Printf("Player ready\n");
        }
        t->Frame = event->Frame - event->ByteAt(0x0D);
        Unknown_0062E5C0(their, index);
    }
    if ((unsigned short)event->ShortAt(0x0B) > (unsigned)t->unknown_04) {
        t->unknown_04 = (unsigned short)event->ShortAt(0x0B);
    }

    if (event->Type != 0x19) {
        i = Breakup_Receive_Packet(event, packetlen);
        if (i == -1) {
            Debug_Printf("***** Error DOLIST_FULL! ******\n");
            return RC_DOLIST_FULL;
        }
        if (event->Type == 0x1C && i > 0) {
            i--;
        }
        Debug_Printf("Frame %d, Receive packet uncompressed into %d commmands\n", ::Frame, i);
        t->unknown_08 += (unsigned short)i;
    } else {
    if (unknown_session_a3d298.Type == 4 || unknown_session_a3d298.Type == 3) {
        Debug_Printf("Received FRAMESYNC packet from %s. Frame = %d\n", unknown_ipx_manager_a40978.Connection_Name(id),
            event->Frame - event->ByteAt(0x0D));
    }
    if (event->LongAt(0x07) != unknown_crc_a40770) {
        return RC_SCENARIO_MISMATCH;
    }
    t->unknown_0c = timer->Timer - timer->Started;
    }
    }
    return retcode;
}

static int Breakup_Receive_Packet(void *buf, int bufsize)
{
    int count = 0;

    switch (unknown_session_a3d2ac) {
    case 0:
        count = Extract_Uncompressed_Events(buf, bufsize);
        break;
    default:
        count = Extract_Compressed_Events(buf, bufsize);
        break;
    }
    return count;
}

static int Extract_Uncompressed_Events(void *buf, int bufsize)
{
    int count = 0;
    int pos = 0;
    int leftover = bufsize;
    EventClass *event;

    while (leftover >= sizeof(EventClass)) {
        event = (EventClass *)(((char *)buf) + pos);
        if (event->Type != 0x19) {
            event->IsExecuted = 0;
            if (event->Type == 0x1F) {
                event->Data.LongsCell.Long1 = (long)new char[event->Data.LongsCell.Long2];
                memcpy((void *)event->Data.LongsCell.Long1, ((char *)buf) + sizeof(EventClass), event->Data.LongsCell.Long2);
                pos += event->Data.LongsCell.Long2;
                leftover -= event->Data.LongsCell.Long2;
            }
            if (!DoList.Add(*event)) {
                if (event->Type == 0x1F) {
                    delete [] (char *)event->Data.LongsCell.Long1;
                }
                return -1;
            }
            count++;
        }
        pos += sizeof(EventClass);
        leftover -= sizeof(EventClass);
    }
    return count;
}


// RetcodeType __fastcall Process_Serial_Packet(char *multi_packet_buf, int first_time)
// (0x00626B30; Red Alert's Process_Serial_Packet).
#include "shared/rules.hpp"
#include "shared/sound_play.hpp"
#include "shared/gadget.hpp"

RetcodeType __fastcall Process_Serial_Packet(char *multi_packet_buf, int first_time)
{
    UnknownSerialPacket *serial_packet = (UnknownSerialPacket *)multi_packet_buf;
    int player_gone = 0;

    if (first_time) {
        if (serial_packet->Command == 0x66) {
            player_gone = 1;
        }
    } else {
        if (serial_packet->Command == 0x66 || serial_packet->Command == 0x69 || serial_packet->Command == 0x6A) {
            player_gone = 1;
        }
    }
    if (player_gone) {
        Destroy_Null_Connection(serial_packet->ScenarioInfo.Color, 0);
        return RC_PLAYER_LEFT;
    }

    if (serial_packet->Command == 0x68) {
        wchar_t *name = Narrow_To_Wide_006fa180(serial_packet->Name);
        if (!unknown_session_a3d298.Messages.Concat_Message(name, serial_packet->ID,
                serial_packet->Message, Rules->MessageDelay * 900)) {
            name = Narrow_To_Wide_006fa180(serial_packet->Name);
            unknown_session_a3d298.Messages.Add_Message(name, serial_packet->ID,
                serial_packet->Message, serial_packet->ID, 0x4046, Rules->MessageDelay * 900, false);
            Voc_Play(Rules->IncomingMessage, 1.0f, 0x2000, 0);
        }
        if (wcslen(serial_packet->Message)) {
            wcscpy(unknown_last_message_a3f122, serial_packet->Message);
        }
        Map.Flag_To_Redraw(2);
        return RC_SERIAL_PROCESSED;
    }

    if (serial_packet->Command == 0x78) {
        if ((unknown_flag_a3f120 && unknown_session_a3d298.Type == 3) || (unknown_flag_a3f121 && unknown_session_a3d298.Type == 4)) {
            Unknown_Play_Taunt_00715fb0(serial_packet->Taunt);
            return RC_SERIAL_PROCESSED;
        }
        return RC_SERIAL_PROCESSED;
    }

    if ((serial_packet->Command >= 100 && serial_packet->Command < 0x6D)
        || (serial_packet->Command >= 0x6E && serial_packet->Command <= 0x74)
        || unknown_session_a3d55c == 1) {
        return RC_SERIAL_PROCESSED;
    }

    EventClass *event = (EventClass *)multi_packet_buf;
    if (event->Type <= 0 || event->Type >= 0x2E) {
        return RC_SERIAL_PROCESSED;
    }
    if ((char)event->House == unknown_player_a35db4->ArrayIndex) {
        return RC_HUNG_UP;
    }
    return RC_NORMAL;
}


// void __fastcall Unknown_00626FA0(HWND list) (0x00626FA0): trims a list box to 50 lines and shows the last.

void __fastcall Unknown_00626FA0(HWND list)
{
    int count = SendMessageA(list, 0x018B, 0, 0);           // LB_GETCOUNT
    if (count > 50) {
        SendMessageA(list, 0x0182, 0, 0);    // LB_DELETESTRING
        count--;
    }
    SendMessageA(list, 0x0197, count - 1, 0);       // LB_SETTOPINDEX
}


// Not exact (RA2_NON_MATCHING): Unknown_00626CB0 at 0x00626CB0, best score 75.78, 2026-11-13; differs: 220 instructions in the original, 226 in the draft (shape 93.72)
#ifdef RA2_NON_MATCHING
// void __fastcall Unknown_00626CB0(HWND dialog) (0x00626CB0): the reconnect dialog's response-time bars:
// green, yellow past 240 ticks, red past 480 (blue while the last ping is under 420 ticks old), shorter
// as the time grows.
#include "shared/surface.hpp"
#include "shared/rect.hpp"
#include "shared/win32_window.hpp"
#include "shared/rgb_class.hpp"

extern int unknown_aabe90;     // 0x00AABE90
extern RGBClass unknown_bar_color_ab6831;     // 0x00AB6831

void __fastcall Unknown_00626CB0(HWND dialog)
{
    for (int i = 0; i < unknown_session_a3d298.Players.Count(); i++) {
        RECT bar_winrect;
        Window_Rect_In_Main_Client(GetDlgItem(dialog, unknown_timing_bars_7ec418[i]), &bar_winrect);

        Rect bar_rect;
        bar_rect.X = bar_winrect.left;
        bar_rect.Y = bar_winrect.top;
        bar_rect.Width = bar_winrect.right - bar_winrect.left;
        bar_rect.Height = bar_winrect.bottom - bar_winrect.top;
        Rect outline(bar_rect.X, bar_rect.Y, bar_rect.Width, bar_rect.Height);

        int id = unknown_session_a3d298.Players[i]->ID;
        int index;
        if (unknown_session_a3d298.Type == 1 || unknown_session_a3d298.Type == 2) {
            index = 0;
        } else {
            index = unknown_ipx_manager_a40978.Connection_Index(id);
        }
        unsigned int progress = 0;
        if (i != 0) {
            progress = unknown_aabe90 - unknown_timing_aabcf0[index].unknown_0c;
        }

        unsigned short color = Build_Hicolor_Pixel(0, 200, 0);
        if (progress > 240) {
            color = Build_Hicolor_Pixel(200, 200, 0);
            if (progress > 480) {
                color = Build_Hicolor_Pixel(200, 0, 0);
                unsigned int age = (timeGetTime() * 60 - unknown_timing_aabcf0[index].unknown_14 * 60) / 1000;
                if (age < 420) {
                    progress = age;
                    color = Build_Hicolor_Pixel(0, 0, 200);
                }
            }
        }

        int w = (100 - (int)(100 * progress / 1200) > 0 ? 100 - (int)(100 * progress / 1200) : 0) * bar_rect.Width;
        bar_rect.Width = (6 > w / 100 ? 6 : w / 100);

        int outline_color = Build_Hicolor_Pixel(unknown_bar_color_ab6831.Red, unknown_bar_color_ab6831.Green, unknown_bar_color_ab6831.Blue);
        unknown_surface_839958->Draw_Rect(unknown_surface_839958->Get_Rect(), outline, outline_color);
        Rect inside = bar_rect;
        inside.X += 2;
        inside.Y += 2;
        inside.Width -= 4;
        inside.Height -= 4;
        unknown_surface_839958->Fill_Rect(unknown_surface_839958->Get_Rect(), inside, color);

    }
}
#endif

// void __fastcall Propose_Kick_Player(HWND dialog, int index) (0x00626FE0; named by its trace).

void __fastcall Propose_Kick_Player(HWND dialog, int index)
{
    if (index < 0 || index >= unknown_session_a3d298.Players.Count()) {
        return;
    }
    Debug_Printf("Propose_Kick_Player %d - %s. Local id is %d\n", index, unknown_session_a3d298.Players[index]->Name,
        unknown_session_a3d298.Players[0]->ID);
    HWND list = GetDlgItem(dialog, 0x651);
    if (index == 0) {
        _DrawMessage_005c2da0(-1, StringTable::Load_String("TXT_RECONNECT_KICK_SELF", 0, "D:\\Ra2\\Queue.CPP", 3378), list);
        Unknown_00626FA0(list);
        return;
    }
    if (unknown_session_a3d298.Type == 4 && unknown_b2a7ec) {
        _DrawMessage_005c2da0(-1, StringTable::Load_String("TXT_CANT_KICK", 0, "D:\\Ra2\\Queue.CPP", 3388), list);
        Unknown_00626FA0(list);
        return;
    }

    UnknownGlobalPacket packet;
    packet.Command = 0x1C;
    strncpy(packet.Name, Wide_To_Narrow_006fa120(unknown_session_a3d298.Players[0]->Name), 20);
    packet.Kick.Proposer = unknown_session_a3d298.Players[0]->ID;
    packet.Kick.Kickee = unknown_session_a3d298.Players[index]->ID;
    for (int i = 1; i < unknown_session_a3d298.Players.Count(); i++) {
        Debug_Printf("Sending kick proposal to %s\n", unknown_session_a3d298.Players[i]->Name);
        unknown_ipx_manager_a40978.Send_Global_Message(&packet, 0x1C7, 1, &unknown_session_a3d298.Players[i]->Address, 0, 0);
    }
    Unknown_00627210(unknown_session_a3d298.Players[0]->ID, unknown_session_a3d298.Players[index]->ID);
}


// void __fastcall Unknown_00627190(UnknownGlobalPacket *packet, IPXAddressClass *address) (0x00627190).

void __fastcall Unknown_00627190(UnknownGlobalPacket *packet, IPXAddressClass *address)
{
    UnknownGlobalPacket *copy = new UnknownGlobalPacket;
    *copy = *packet;
    unknown_session_a3d298.GlobalPackets_8a0.Add(copy);
}


// void __fastcall Unknown_00627210(int id, int index) (0x00627210): a kick vote, shown in the reconnect dialog.

void __fastcall Unknown_00627210(int id, int index)
{
    int i;
    wchar_t buffer[256];
    wchar_t *name;

    for (i = 0; i < unknown_session_a3d298.KickVotes[index]; i++) {
        if (unknown_session_a3d298.KickVoters[index].ID[i] == id) {
            return;
        }
    }
    unknown_session_a3d298.KickVoters[index].ID[unknown_session_a3d298.KickVotes[index]] = id;
    unknown_session_a3d298.KickVotes[index]++;
    if (unknown_session_a3d298.Type == 3 || unknown_session_a3d298.Type == 4) {
        name = unknown_ipx_manager_a40978.Connection_Name(id);
        if (!name) {
            name = unknown_session_a3d298.Players[0]->Name;
        }
        Debug_Printf("Player %s votes to kick player %s from the game\n", name, unknown_ipx_manager_a40978.Connection_Name(index));
        swprintf(buffer, StringTable::Load_String("TXT_RECONNECT_KICK_RECEIVED", 0, "D:\\Ra2\\Queue.CPP", 3488), name, unknown_ipx_manager_a40978.Connection_Name(index));
        HWND dialog = Unknown_00735B10();
        HWND list = GetDlgItem(dialog, 0x651);
        _DrawMessage_005c2da0(-1, buffer, list);
        Unknown_00626FA0(list);
    }
}


// BOOL __stdcall Unknown_00627350(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam) (0x00627350):
// the reconnect dialog's window procedure.
#include "shared/mouse.hpp"

BOOL __stdcall Unknown_00627350(HWND dialog, UINT message, WPARAM wparam, LPARAM lparam)
{
    int *result = (int *)GetWindowLongA(dialog, 8);     // DWL_USER
    HWND button;
    HWND bar;
    int i;

    switch (message) {
    case 0x0002:        // WM_DESTROY
        Unknown_005B6060(dialog);
        return FALSE;
    case 0x002B:        // WM_DRAWITEM
        Unknown_005FE380(lparam);
        return TRUE;
    case 0x0014:        // WM_ERASEBKGND
        return TRUE;
    case 0x000F:        // WM_PAINT
        Unknown_005FEE70(dialog);
        Unknown_00626CB0(dialog);
        ValidateRect(dialog, 0);
        return FALSE;
    case 0x0111:        // WM_COMMAND
        switch ((unsigned short)wparam) {
        case 2:         // IDCANCEL
            *result = 2;
            return FALSE;
        case 0x640:
            Propose_Kick_Player(dialog, 0);
            return FALSE;
        case 0x642:
            Propose_Kick_Player(dialog, 1);
            return FALSE;
        case 0x641:
            Propose_Kick_Player(dialog, 2);
            return FALSE;
        case 0x643:
            Propose_Kick_Player(dialog, 3);
            return FALSE;
        case 0x644:
            Propose_Kick_Player(dialog, 4);
            return FALSE;
        case 0x645:
            Propose_Kick_Player(dialog, 5);
            return FALSE;
        case 0x646:
            Propose_Kick_Player(dialog, 6);
            return FALSE;
        case 0x647:
            Propose_Kick_Player(dialog, 7);
            return FALSE;
        }
        break;
    case 0x0113:        // WM_TIMER
        if (unknown_session_a3d298.Type == 4 && wparam == 1 && unknown_ladder_client_a72d00) {
            if (unknown_b2a630 < 5) {
                int handle;
                unknown_ladder_client_a72d00->Request_Ping(unknown_game_address_b2a758, 1000, &handle);
                unknown_b2a630++;
            }
            unknown_ab62f8++;
            if (unknown_b2a7ec) {
                if (!unknown_ab62f4 && unknown_ab62f8 > 5 && !unknown_b2aaf8) {
                    unknown_ab62f4 = true;
                    unknown_ab62ec++;
                } else if (!unknown_ab62f5 && unknown_ab62f8 > 10 && !unknown_b2aaf8) {
                    unknown_ab62f5 = true;
                    unknown_ab62f0++;
                }
                if (unknown_ab62ec > 5 || unknown_ab62f0 > 2) {
                    unknown_ab62e0 = true;
                    *result = 2;
                    return FALSE;
                }
            }
            SetTimer(dialog, 1, 1000, 0);
        }
        break;
    case 0x0132:        // WM_CTLCOLORMSGBOX .. WM_CTLCOLORSTATIC
    case 0x0133:
    case 0x0134:
    case 0x0135:
    case 0x0136:
    case 0x0137:
    case 0x0138:
        return (BOOL)GetStockObject(4);      // BLACK_BRUSH
    case 0x0497: {      // WM_USER + 0x97: set up
        Unknown_005B6000(dialog);
        for (i = 0; i < 8; i++) {
            button = GetDlgItem(dialog, unknown_kick_buttons_7ec3f8[i]);
            EnableWindow(button, FALSE);
            bar = GetDlgItem(dialog, unknown_timing_bars_7ec418[i]);
            EnableWindow(bar, FALSE);
        }
        for (i = 0; i < 8; i++) {
            button = GetDlgItem(dialog, unknown_kick_buttons_7ec3f8[i]);
            bar = GetDlgItem(dialog, unknown_timing_bars_7ec418[i]);
            if (i < unknown_session_a3d298.Players.Count()) {
                SendMessageA(button, 0x04B2, 0, (LPARAM)unknown_session_a3d298.Players[i]);
                EnableWindow(button, TRUE);
                EnableWindow(bar, TRUE);
            } else {
                DestroyWindow(button);
                DestroyWindow(bar);
            }
        }
        if (unknown_session_a3d298.Type == 4) {
            unknown_b2a630 = 0;
            unknown_b2aaf8 = 0;
            unknown_ab62f4 = false;
            unknown_ab62f5 = false;
            unknown_ab62f8 = 0;
            unknown_ab62e0 = false;
            SetTimer(dialog, 1, 1000, 0);
        }
        WWMouse->Show_Mouse();
        break;
    }
    }
    return FALSE;
}


// void __fastcall Kick_Player_Now(ConnManClass *net, int index, FrameSyncStruct *their, unsigned char error) (0x00627740).

void __fastcall Kick_Player_Now(ConnManClass *net, int index, FrameSyncStruct *their, unsigned char error)
{
    bool recent = false;
    int id;
    int i;

    if (time(0) - unknown_last_kick_ab6304 < 60) {
        recent = true;
    }
    unknown_last_kick_ab6304 = time(0);
    id = net->Connection_ID(index);
    Debug_Printf("Kicking player %d from the game - Frame is %d\n", id, ::Frame);
    if (id == -1) {
        Debug_Printf("Kick_Player_Now bailing - bad house ID\n");
        return;
    }
    Houses[id]->unknown_150 = true;
    if (Unknown_005C3150(Houses[id]) == 1 && unknown_game_mode_a3d298 == 4 && !unknown_flag_a417c8) {
        Unknown_0069A160();
        unknown_flag_abd500 = true;
        if (!recent) {
            unknown_flag_a3d8d1 = true;
        }
        for (i = 0; i < Houses.Count(); i++) {
            HouseClass *house = Houses[i];
            if (house && !house->unknown_13d && house->unknown_134) {
                house->unknown_150 = true;
            }
        }
        Unknown_00698D00(false);
    }
    if (Unknown_005C3150(Houses[id]) > 1 && !recent) {
        Houses[id]->unknown_18e = true;
    }
    for (i = index; i < net->Num_Connections() - 1; i++) {
        their[i] = their[i + 1];
        unknown_session_a3d298.KickVotes[i] = unknown_session_a3d298.KickVotes[i + 1];
        unknown_session_a3d298.KickVoters[i] = unknown_session_a3d298.KickVoters[i + 1];
    }
    Destroy_Connection_005bb5d0(id, error);
}


// int __fastcall Handle_Timeout(ConnManClass *net, FrameSyncStruct *their) (0x00627910): reconnect a modem game or kick the oldest network player.

int __fastcall Handle_Timeout(ConnManClass *net, FrameSyncStruct *their)
{
    int oldest_index;
    int i, j;

    if (unknown_game_mode_a3d298 == 1 || unknown_game_mode_a3d298 == 2) {
        if (net->Num_Connections()) {
            unknown_counter_a3fabc++;
            if (!Reconnect_Modem()) {
                unknown_counter_a3fabc--;
                return 0;
            }
            unknown_counter_a3fabc--;
            return 1;
        }
    } else if (unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) {
        j = 0x7FFFFFFF;
        oldest_index = 0;
        for (i = 0; i < net->Num_Connections(); i++) {
            if (their[i].Frame < j) {
                j = their[i].Frame;
                oldest_index = i;
            }
        }
        Kick_Player_Now(net, oldest_index, their, 1);
    }
    return 1;
}


// Saved repeated-megamission unit lists; original006279D0.
#include "shared/debug_printf.hpp"
bool __fastcall Unknown_006279D0(unsigned char *count, int *size, char *whom, char *units, bool save)
{
    bool matched = false;
    if (*count > 1) {
        for (int i = 0; i < 8; ++i) {
            int saved_count = unknown_megamission_counts_aabdb0[i];
            if (*count == saved_count &&
                memcmp(unknown_megamission_lists_a76ca8[i], whom, 5) == 0 &&
                memcmp(unknown_megamission_lists_a76ca8[i] + 5, units,
                    saved_count * 5 - 5) == 0) {
                if (save) *size += 5 - *count * 5;
                matched = true;
                if (save) {
                    *count = 0;
                    whom[6] = (char)i;
                }
                goto finished;
            }
        }
    }
    if (save && *count > 1) {
        int oldest = -1;
        int greatest_age = -1;
        for (int slot = 0; slot < 8; ++slot) {
            if (unknown_megamission_frames_aabe94[slot] == 0) {
                oldest = slot;
                break;
            }
            int age = Frame - unknown_megamission_frames_aabe94[slot];
            if (age > greatest_age) {
                greatest_age = age;
                oldest = slot;
            }
        }
        if (oldest == -1) oldest = 0;
        Debug_Printf("Compress: Saving new megamission list with %d units to slot %d\n", *count, oldest);
        unknown_megamission_frames_aabe94[oldest] = Frame;
        memcpy(unknown_megamission_lists_a76ca8[oldest], whom, 5);
        memcpy(unknown_megamission_lists_a76ca8[oldest] + 5, units, *count * 5 - 5);
        unknown_megamission_counts_aabdb0[oldest] = *count;
        whom[6] = (char)oldest;
    }
finished:
    return matched;
}


// int __fastcall Add_Compressed_Events(void *buf, int bufsize, int frame_delay, int size, int cap, int *added) (0x00627B60)
#include "shared/mission.hpp"
extern unsigned char unknown_event_lengths_7d659c[];
extern bool unknown_flag_a40d1c;    // 0x00A40D1C (trace the events)

#ifndef RA2_QUEUE_SAME_TARGET
#define RA2_QUEUE_SAME_TARGET
inline bool Same_Target(TargetClass const &a, TargetClass const &b)
{
    return a.Kind == b.Kind && a.ID == b.ID;
}
#endif

int __fastcall Add_Compressed_Events(void *buf, int bufsize, int frame_delay, int size, int cap, int *added)
{
    int num = 0;
    unsigned char eventtype;
    EventClass prevevent;
    int datasize;
    int storedsize;
    unsigned char *unitsptr = 0;
    unsigned char numunits = 0;
    bool missiondup = false;
    bool cached = false;
    char mission;
    char *units;
    char *whom;

    memset(&prevevent, 0, sizeof(EventClass));
    units = 0;
    whom = 0;

    if (unknown_flag_a40d1c) {
        Debug_Printf("\nFrame %d: Building Send Packet\n", ::Frame);
    }

    while (OutList.Count && num < cap) {
        eventtype = OutList.First().Type;
        datasize = unknown_event_lengths_7d659c[eventtype];
        if (eventtype == 0x1F) {
            storedsize = datasize + 1 + OutList.First().LongAt(0x0B);
        } else {
            storedsize = datasize + 1;
        }

        if (prevevent.Type == 4) {
            if (eventtype == 4) {
                if (!cached
                    && (mission = OutList.First().ByteAt(0x0C)) == (char)prevevent.ByteAt(0x0C)
                    && Same_Target(OutList.First().TargetAt(0x0E), prevevent.TargetAt(0x0E))
                    && Same_Target(OutList.First().TargetAt(0x13), prevevent.TargetAt(0x13))) {
                    if (unknown_flag_a40d1c) {
                        Debug_Printf("      adding Whom:%x Mission:%s Target:%x Dest:%x\n",
                            OutList.First().TargetAt(0x07).Encode(),
                            MissionClass::Name_From_Mission(mission),
                            OutList.First().TargetAt(0x0E).Encode(),
                            OutList.First().TargetAt(0x13).Encode());
                    }
                    datasize = 5;
                    storedsize = datasize;
                    numunits++;
                    missiondup = true;
                } else {
                    if (unknown_flag_a40d1c) {
                        Debug_Printf("  New MEGAMISSION run:\n");
                    }
                    Debug_Printf("Compress: New megamission run - checking for previous repeat of %d units\n", numunits);
                    Unknown_006279D0(&numunits, &size, whom, units, true);
                    *unitsptr = numunits;
                    unitsptr = (unsigned char *)buf + size + 1;
                    storedsize += 1;
                    numunits = 1;
                    missiondup = false;
                }
            } else {
                Debug_Printf("Compress: Non-megamission event - checking for previous repeat of %d units\n", numunits);
                Unknown_006279D0(&numunits, &size, whom, units, true);
                *unitsptr = numunits;
                unitsptr = 0;
                numunits = 0;
                missiondup = false;
            }
        } else if (eventtype == 4) {
            if (unknown_flag_a40d1c) {
                Debug_Printf("  New MEGAMISSION run:\n");
            }
            unitsptr = (unsigned char *)buf + size + 1;
            storedsize += 1;
            numunits = 1;
            missiondup = false;
        }
        cached = false;

        if (size + storedsize > bufsize) {
            if (eventtype == 4 && missiondup) {
                numunits--;
                Debug_Printf("Compress: Out of buffer space at %d units\n", numunits);
                Unknown_006279D0(&numunits, &size, whom, units, true);
                *unitsptr = numunits;
            }
            break;
        }

        if (unknown_session_a3d2ac == 2) {
            OutList.First().Frame = ((::Frame + frame_delay + (unknown_frame_send_rate_a3d564 - 1)) / unknown_frame_send_rate_a3d564)
                * unknown_frame_send_rate_a3d564;
        } else {
            OutList.First().Frame = ::Frame + frame_delay;
        }
        OutList.First().House = unknown_player_a35db4->ArrayIndex;
        OutList.First().IsExecuted = 0;

        if (!DoList.Add(OutList.First(), OutList.Timings[OutList.Head])) {
            break;
        }

        switch (eventtype) {
        case 4:
            if (missiondup) {
                *unitsptr = numunits;
                memcpy((char *)buf + size, &OutList.First().TargetAt(0x07), datasize);
                size += datasize;
                if (OutList.Count != 1 && num != cap - 1) {
                    if (Unknown_006279D0(&numunits, &size, whom, units, false)) {
                        Debug_Printf("Compress: Found early repeat megamission - splitting after %d units\n", numunits);
                        cached = true;
                        *unitsptr = numunits;
                    }
                } else {
                    Debug_Printf("Compress: Ended megamission run at %d units - no more events\n", numunits);
                    Unknown_006279D0(&numunits, &size, whom, units, true);
                    *unitsptr = numunits;
                }
            } else {
                *unitsptr = numunits;
                *((char *)buf + size) = 4;
                whom = (char *)buf + size + 2;
                memcpy(whom, &OutList.First().Data, datasize);
                units = whom + datasize;
                size += datasize + 2;
            }
            break;

        case 0x1B:
            *((char *)buf + size) = 0x1B;
            memcpy((char *)buf + size + 1, &OutList.First().ByteAt(0x0D), datasize);
            size += datasize + 1;
            break;

        case 0x1F:
            *((char *)buf + size) = 0x1F;
            memcpy((char *)buf + size + 1, &OutList.First().LongAt(0x0B), datasize);
            size += datasize + 1;
            memcpy((char *)buf + size, (void *)OutList.First().LongAt(0x07), OutList.First().LongAt(0x0B));
            size += OutList.First().LongAt(0x0B);
            break;

        default:
            *((char *)buf + size) = eventtype;
            memcpy((char *)buf + size + 1, &OutList.First().Data, datasize);
            size += datasize + 1;
            break;
        }

        num++;
        memcpy(&prevevent, &OutList.First(), sizeof(EventClass));
        if (!OutList.Next()) {
            break;
        }
    }

    *added = num;
    return size;
}


// Not exact (RA2_NON_MATCHING): Extract_Compressed_Events at 0x006282D0, best score 32.83, 2026-11-13; differs: 414 instructions in the original, 506 in the draft (shape 74.78)
#ifdef RA2_NON_MATCHING
// int __fastcall Extract_Compressed_Events(void *buf, int bufsize) (0x006282D0; Red Alert's
// Extract_Compressed_Events with Red Alert 2's megamission unit lists).
// NATIVE (Scout1209 r10): the type 0x1F data travels as a handle in the 32-bit field at +0x07
// (native/link/native_event_data.cpp); the size stays at +0x0B, as in the original.
int Native_Event_Data_Handle(void *data);
void *Native_Event_Data_Release(int handle);
extern unsigned char unknown_event_lengths_7d659c[];
extern xTargetClass unknown_unit_lists_aabed0[64][128];
extern int unknown_unit_counts_a76b74[64];

// NATIVE (Scout1223): a stack-protector abort here ended a two-Mac LAN game (user crash report
// 2026-11-10): a record whose type or length runs past the packet or the event (a type past the
// 48-entry length table, a megamission slot or unit count past the lists) overran `eventdata`.
// Such a packet is logged with its bytes and decoding stops at that record; the parse is
// otherwise unchanged.
void Native_Malformed(void const *buf, int bufsize, int pos, int type, char const *why);   // native/link/native_event_data.cpp
#define NATIVE_CHECK(cond, why) do { if (!(cond)) { Native_Malformed(buf, bufsize, pos, eventdata.Type, why); return count; } } while (0)

int __fastcall Extract_Compressed_Events(void *buf, int bufsize)
{
    int pos = 0;
    int leftover = bufsize;
    EventClass *event;
    int count = 0;
    int datasize = 0;
    EventClass eventdata;
    unsigned char numunits = 0;
    int house;
    int slot;
    int units;
    xTargetClass *list;
    char *src;

    memset(&eventdata, 0, sizeof(EventClass));
    house = ((EventClass *)buf)->House;
    datasize = 13;
    event = (EventClass *)(((char *)buf) + pos);

    while ((unsigned)leftover >= (datasize + sizeof(eventdata.Type))) {
        if (event->Type != 0x19) {
            if (event->Type == 0x1C) {
                eventdata.House = event->House;
                eventdata.Frame = event->Frame;
                pos += 6;
                leftover -= 6;
            } else if (event->Type == 4) {
                numunits = *(((unsigned char *)buf) + pos + sizeof(eventdata.Type));
                pos += sizeof(numunits);
                leftover -= sizeof(numunits);
            }
            memset(&eventdata.Data, 0, sizeof(eventdata.Data));
            eventdata.Type = event->Type;
            NATIVE_CHECK(eventdata.Type < 48, "type");
            datasize = unknown_event_lengths_7d659c[eventdata.Type];
            NATIVE_CHECK(pos + 1 + datasize <= bufsize, "length");

            switch (eventdata.Type) {
            case 0x1B:
                memcpy(&eventdata.Data.Bytes[6], ((char *)buf) + pos + sizeof(eventdata.Type), datasize);
                break;

            case 0x1F:
                NATIVE_CHECK(datasize <= 4, "0x1F size field");
                memcpy(&eventdata.LongAt(0x0B), ((char *)buf) + pos + sizeof(eventdata.Type), datasize);
                NATIVE_CHECK(eventdata.LongAt(0x0B) >= 0 && pos + 1 + datasize + eventdata.LongAt(0x0B) <= bufsize, "0x1F data");
                {
                    char *variable = new char[eventdata.LongAt(0x0B)];
                    eventdata.LongAt(0x07) = Native_Event_Data_Handle(variable);
                    memcpy(variable, ((char *)buf) + pos + sizeof(eventdata.Type) + datasize, eventdata.LongAt(0x0B));
                }
                pos += eventdata.LongAt(0x0B);
                leftover -= eventdata.LongAt(0x0B);
                break;

            case 4:
                memcpy(&eventdata.Data, ((char *)buf) + pos + sizeof(eventdata.Type), datasize);
                if (numunits > 1) {
                    pos += (datasize + sizeof(eventdata.Type));
                    leftover -= (datasize + sizeof(eventdata.Type));
                    datasize = sizeof(eventdata.Data.MegaMissionF.Whom);
                    Debug_Printf("Decoding new MEGAMISSION\n");
                    slot = (char)eventdata.Data.Bytes[6] + house * 8;
                    NATIVE_CHECK(slot >= 0 && slot < 64 && numunits <= 128 && pos + (numunits - 1) * 5 <= bufsize, "unit list");
                    unknown_unit_lists_aabed0[slot][0] = eventdata.Data.MegaMissionF.Whom;
                    units = 1;
                    unknown_unit_counts_a76b74[slot] = units;
                    if (eventdata.Frame == 0 || eventdata.Type == 0) {
                        Debug_Printf("Frame %d:Empty megamission decoded\n", ::Frame);
                    }
                    if (numunits) {
                        src = ((char *)buf) + pos;
                        list = &unknown_unit_lists_aabed0[slot][1];
                        do {
                            if (!DoList.Add(eventdata)) {
                                return -1;
                            }
                            count++;
                            *list = *(xTargetClass *)src;
                            eventdata.Data.MegaMissionF.Whom = *list++;
                            numunits--;
                            units++;
                            unknown_unit_counts_a76b74[slot] = units;
                            if (numunits == 1) {
                                datasize -= sizeof(eventdata.Type);
                                Debug_Printf("Got new unit list for player %d with %d units in slot %d\n", house, units, (char)eventdata.Data.Bytes[6]);
                                break;
                            }
                            pos += datasize;
                            src += datasize;
                            leftover -= datasize;
                        } while (numunits);
                    }
                } else if (numunits == 0) {
                    pos += (datasize + sizeof(eventdata.Type));
                    leftover -= (datasize + sizeof(eventdata.Type));
                    datasize = sizeof(eventdata.Data.MegaMissionF.Whom);
                    units = 1;
                    slot = (char)eventdata.Data.Bytes[6] + house * 8;
                    NATIVE_CHECK(slot >= 0 && slot < 64 && unknown_unit_counts_a76b74[slot] <= 128, "saved unit list");
                    numunits = unknown_unit_counts_a76b74[slot];
                    eventdata.Data.MegaMissionF.Whom = unknown_unit_lists_aabed0[slot][0];
                    if (eventdata.Frame == 0 || eventdata.Type == 0) {
                        Debug_Printf("Frame %d:Empty megamission decoded from slot %d. Player %d\n", ::Frame, (char)eventdata.Data.Bytes[6], house);
                    }
                    if (numunits) {
                        list = &unknown_unit_lists_aabed0[slot][1];
                        do {
                            if (!DoList.Add(eventdata)) {
                                return -1;
                            }
                            count++;
                            numunits--;
                            units++;
                            eventdata.Data.MegaMissionF.Whom = *list++;
                            if (numunits == 1) {
                                datasize = -1;
                                Debug_Printf("Used old unit list in slot %d - %d bytes saved\n", (char)eventdata.Data.Bytes[6], units * 5);
                                break;
                            }
                        } while (numunits);
                    }
                }
                break;

            default:
                memcpy(&eventdata.Data, ((char *)buf) + pos + sizeof(eventdata.Type), datasize);
                break;
            }

            if (eventdata.Frame == 0 || eventdata.Type == 0) {
                Debug_Printf("Frame %d: Empty event being added to queue\n", ::Frame);
            }
            if (!DoList.Add(eventdata)) {
                if (eventdata.Type == 0x1F) {
                    delete[] (char *)Native_Event_Data_Release(eventdata.LongAt(0x07));
                }
                return -1;
            }
            count++;
            pos += (datasize + sizeof(eventdata.Type));
            leftover -= (datasize + sizeof(eventdata.Type));
            // The original leaves when the packet is used up (0x006287FE, `je` on the subtraction):
            // the earlier draft fell through to the loop test with the record's datasize, which is
            // -1 after a cached unit list ("Used old unit list"), so 0 >= 0 ran one more pass over
            // the stale bytes after the packet (garbage events, then a stack overrun: the two-Mac
            // LAN crash, Scout1231).
            if (leftover == 0) {
                return count;
            }
            event = (EventClass *)(((char *)buf) + pos);
            datasize = unknown_event_lengths_7d659c[event->Type];
            if (event->Type == 4) {
                datasize += sizeof(numunits);
            }
        } else {
            pos += (datasize + sizeof(eventdata.Type));
            leftover -= (datasize + sizeof(eventdata.Type));
            event = (EventClass *)(((char *)buf) + pos);
            datasize = 13;
        }
    }
    return count;
}
#endif

// int __fastcall Execute_DoList(int max_houses, int base_house, ConnManClass *net,
//     CDTimerClass<FrameTimerClass> *skip_crc, FrameSyncStruct *their) (0x00628880; Red Alert's Execute_DoList)
extern int unknown_max_ahead_frame2_a3d23c;     // 0x00A3D23C (Red Alert's NewMaxAheadFrame2)
extern int unknown_max_ahead_frame1_a3d258;     // 0x00A3D258 (Red Alert's NewMaxAheadFrame1)
extern int unknown_exit_frame1_a402e0;          // 0x00A402E0
extern int unknown_exit_frame2_a402e4;          // 0x00A402E4
extern int unknown_exit_house_a402e8;           // 0x00A402E8 (-1: none)
extern bool unknown_debug_print_events_a40d1c;  // 0x00A40D1C
// Executes the events of a run of identical megamission events of the queue 0x00A35F30 (0x00629290).
void __fastcall Unknown_00629290(int start, int count);

int __fastcall Execute_DoList(int max_houses, int base_house, ConnManClass *net,
    CDTimerClass<FrameTimerClass> *skip_crc, FrameSyncStruct *their)
{
    int count = DoList.Count;
    int house_count = Houses.Count();
    int i, j, k;
    int index;
    int check_crc;
    HouseClass *hptr;

    for (j = 0; j < count; j++) {
        if (DoList[j].Type != 0x1C && DoList[j].Frame > unknown_max_ahead_frame1_a3d258 &&
                DoList[j].Frame < unknown_max_ahead_frame2_a3d23c) {
            Debug_Printf("DoList: Moving event from frame %d to frame %d\n", DoList[j].Frame, unknown_max_ahead_frame2_a3d23c);
            DoList[j].Frame = unknown_max_ahead_frame2_a3d23c;
        }
    }

    if (unknown_exit_frame1_a402e0 && unknown_exit_frame2_a402e4 && unknown_exit_house_a402e8 != -1) {
        for (j = 0; j < count; j++) {
            if (DoList[j].Type == 0x13 && DoList[j].House == unknown_exit_house_a402e8 &&
                    DoList[j].Frame > unknown_exit_frame1_a402e0 && DoList[j].Frame < unknown_exit_frame2_a402e4) {
                Debug_Printf("DoList: Moving EXIT event from frame %d to frame %d\n", DoList[j].Frame, unknown_exit_frame2_a402e4);
                DoList[j].Frame = unknown_exit_frame2_a402e4;
            }
        }
        if (::Frame > unknown_exit_frame2_a402e4) {
            unknown_exit_frame1_a402e0 = 0;
            unknown_exit_frame2_a402e4 = 0;
            unknown_exit_house_a402e8 = -1;
        }
    }

    for (i = 0; i < house_count; i++) {
        hptr = Houses[i];
        if (!hptr) {
            continue;
        }
        if (!hptr->unknown_134 && !hptr->unknown_135) {
            continue;
        }

        for (j = 0; j < count; j++) {
            if (DoList[j].House == hptr->ArrayIndex && ::Frame >= DoList[j].Frame && !DoList[j].IsExecuted) {
                if (::Frame > DoList[j].Frame && DoList[j].Type != 0x1C &&
                        unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5) {
                    EventClass *event = &DoList[j];
                    Debug_Printf("Packet received too late!\n");
                    Debug_Printf("--------- Event data: -------------------\n");
                    Debug_Printf("Type:       %s\n", EventNames[event->Type]);
                    Debug_Printf("Frame:      %d\n", event->Frame);
                    Debug_Printf("ID:         %d\n", event->House);
                    Debug_Printf("MaxAhead=%d\n", unknown_max_ahead_a3d560);
                    Debug_Printf("Frame=%d\n", ::Frame);
                    Debug_Printf("FrameSendRate=%d\n", unknown_frame_send_rate_a3d564);
                    net->Num_Connections();
                    continue;
                }

                if (DoList[j].Type == 0x13 || DoList[j].Type == 0x0C || DoList[j].Type == 0x22) {
                    if (DoList[j].Type == 0x13) {
                        HouseClass *quithptr = 0;
                        for (int player = 0; player < house_count; player++) {
                            quithptr = Houses[player];
                            if (!quithptr) {
                                continue;
                            }
                            if (quithptr->ArrayIndex == DoList[j].House) {
                                quithptr->unknown_18f = true;
                                break;
                            }
                        }
                        if (Unknown_005C3150(quithptr) == 1 && unknown_game_mode_a3d298 == 4 && !unknown_flag_a417c8) {
                            unknown_flag_a3d8d1 = true;
                            Unknown_0069A160();
                            Unknown_00698D00(false);
                        }
                        if (unknown_game_mode_a3d298 == 4 && !unknown_flag_a417c8 && unknown_player_a35db4 != 0 &&
                                unknown_player_a35db4 == quithptr) {
                            Debug_Printf("Sending game results because I quit, but didn't see completion");
                            Unknown_00698D00(false);
                        }
                    }
                    if (unknown_debug_print_events_a40d1c) {
                        if (DoList[j].Type == 0x13) {
                            Debug_Printf("Exit Event: ID:%d (%s),  Event Frame:%d,  My Frame:%d\n",
                                DoList[j].House, (char const *)Houses[DoList[j].House]->IniName, DoList[j].Frame, ::Frame);
                        }
                    }
                    if (DoList[j].House == unknown_player_a35db4->ArrayIndex) {
                        int stamp = DoList.Timings[(DoList.Head + j) & 0x3FFF];
                        unknown_time_7ec498 = timeGetTime() - stamp;
                        DoList[j].Execute();
                    } else if (DoList[j].Type == 0x13) {
                        Debug_Printf("Processing EXIT event on frame %d\n", ::Frame);
                        if (unknown_game_mode_a3d298 == 1 || unknown_game_mode_a3d298 == 2) {
                            Destroy_Null_Connection(i, 0);
                        } else if ((unknown_game_mode_a3d298 == 3 || unknown_game_mode_a3d298 == 4) && net) {
                            index = net->Connection_Index(i);
                            if (index != -1) {
                                for (k = index; k < net->Num_Connections() - 1; k++) {
                                    their[k] = their[k + 1];
                                }
                                Destroy_Connection_005bb5d0(i, 0);
                            }
                        }
                        if ((unknown_flags_a3f600 & 2) && DoList[j].Type == 0x13) {
                            hptr->unknown_134 = false;
                            hptr->unknown_194 = Rules->MaxIQ;
                            HouseClass::unknown_func_4ec5e0();
                            hptr->IniName = FixedString<21>("Computer");
                            wcsncpy(hptr->UIName, StringTable::Load_String("TXT_COMPUTER", 0, "D:\\Ra2\\Queue.CPP", 0x1623), 0x15);
                            hptr->UIName[20] = 0;
                            Debug_Printf("Removing a player %s:%d\n", "D:\\Ra2\\Queue.CPP", 0x1625);
                            unknown_session_a3d55c--;
                        }
                    }
                } else if (DoList[j].Type == 0x1C) {
                    if (!skip_crc || *skip_crc == 0) {
                        check_crc = 1;
                    } else {
                        check_crc = 0;
                    }
                    if (check_crc && DoList[j].Frame == ::Frame && DoList[j].ByteAt(0x0D) < 256) {
                        index = (DoList[j].Frame - DoList[j].ByteAt(0x0D)) & 0xFF;
                        if (CRC[index] != (unsigned long)DoList[j].LongAt(0x07)) {
                            if (unknown_flag_ab62e8) {
                                // The CRC log of every sync slot (k is reduced to a slot
                                // number before each call; the loop count is separate).
                                k = 0;
                                for (int n = 256; n; n--) {
                                    k &= 0xFF;
                                    Unknown_0062DBA0(k, &DoList[j]);
                                    k++;
                                }
                            } else {
                                Print_CRCs(&DoList[j]);
                            }
                            unknown_a3fabc++;
                            if (WWMessageBox(L" ").Process(StringTable::Load_String("TXT_OUT_OF_SYNC", 0, "D:\\Ra2\\Queue.CPP", 0x1655),
                                    StringTable::Load_String("TXT_STOP", 0, "D:\\Ra2\\Queue.CPP", 0x1656), 0, 0, 0) == 0) {
                                unknown_a3fabc--;
                                return 0;
                            }
                            unknown_a3fabc--;
                            return 0;
                        }
                    }
                } else {
                    int stamp = DoList.Timings[(DoList.Head + j) & 0x3FFF];
                    unknown_time_7ec498 = timeGetTime() - stamp;
                    if (DoList[j].Type == 4) {
                        unknown_event_queue_a35f30.Add(DoList[j]);
                    } else {
                        DoList[j].Execute();
                    }
                }
                DoList[j].IsExecuted = 1;
            }
        }

        int mission = -1;
        TargetClass target;
        TargetClass destination;
        target.Kind = 0;
        destination.Kind = 0;
        int start = 0;
        for (k = 0; k < unknown_event_queue_a35f30.Count; k++) {
            bool differ = false;
            if (mission != -1) {
                EventClass &event = unknown_event_queue_a35f30[k];
                if ((char)event.ByteAt(0x0C) != (char)mission) {
                    differ = true;
                }
                if (event.TargetAt(0x0E).Kind != target.Kind || event.TargetAt(0x0E).ID != target.ID ||
                        event.TargetAt(0x13).Kind != destination.Kind || event.TargetAt(0x13).ID != destination.ID) {
                    differ = true;
                }
            }
            mission = (char)unknown_event_queue_a35f30[k].ByteAt(0x0C);
            target = unknown_event_queue_a35f30[k].TargetAt(0x0E);
            destination = unknown_event_queue_a35f30[k].TargetAt(0x13);
            if (differ) {
                if (k - start > 1) {
                    Unknown_00629290(start, k - start);
                }
                start = k;
            } else if (k == unknown_event_queue_a35f30.Count - 1 && k - start + 1 > 1) {
                Unknown_00629290(start, k - start + 1);
            }
        }
        while (unknown_event_queue_a35f30.Count) {
            unknown_event_queue_a35f30.First().Execute();
            unknown_event_queue_a35f30.Next();
        }
    }
    return 1;
}


// void __fastcall Unknown_00629220(int start, int count, TargetClass whom, TargetClass const &target) (0x00629220)
// Retargets queued events: the first of the events start..start+count of the queue 0x00A35F30 whose target at
// +0x07 is whom gets target as its +0x13 target (name hypothesis; no callers found).

// Kind first, then ID (an inline helper: the original hoists the Kind byte load first).
#ifndef RA2_QUEUE_SAME_TARGET
#define RA2_QUEUE_SAME_TARGET
inline bool Same_Target(TargetClass const &a, TargetClass const &b)
{
    return a.Kind == b.Kind && a.ID == b.ID;
}
#endif

void __fastcall Unknown_00629220(int start, int count, TargetClass whom, TargetClass const &target)
{
    for (int i = start; i <= start + count; i++) {
        if (Same_Target(whom, unknown_event_queue_a35f30[i].TargetAt(0x07))) {
            unknown_event_queue_a35f30[i].TargetAt(0x13) = target;
            return;
        }
    }
}


// void __fastcall Unknown_00629290(int start, int count) (0x00629290; RA2 only, no TibSun twin)
// Spreads a run of identical megamissions aimed at one cell: the foot units of events start..start+count are ordered
// by distance from the unit nearest their centre; the first keeps the cell, every later one walks from it along its
// offset from that unit for up to six cells, and its event gets the first free, level-compatible cell of the same
// movement zone. Units sharing a cell get the same destination. Purpose and names are hypotheses.
#include "shared/building_class.hpp"
#include "shared/unit.hpp"
#include "shared/cell_class.hpp"
#include "shared/path_estimator.hpp"
#include "shared/techno_type.hpp"
#include "shared/ini_class.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/vector3.hpp"
#include "shared/fast_math.hpp"

template <class T>
DynamicVectorClass<T>::DynamicVectorClass(unsigned size, T const *array)
    : VectorClass<T>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
template <class T>
void VectorClass<T>::Clear()
{
    if (Vector && IsAllocated) {
        delete[] Vector;
        Vector = 0;
    }
    IsAllocated = false;
    VectorMax = 0;
}
template <class T>
void DynamicVectorClass<T>::Clear()
{
    ActiveCount = 0;
    VectorClass<T>::Clear();
}
template <class T>
VectorClass<T>::~VectorClass()
{
    VectorClass<T>::Clear();
}

CoordStruct __fastcall Unknown_005D5010(DynamicVectorClass<ObjectClass *> const &objects);
ObjectClass *__fastcall Closest_Object_005d5090(DynamicVectorClass<ObjectClass *> const &objects, CoordStruct const &coord);
void __fastcall Unknown_00629220(int start, int count, TargetClass whom, TargetClass const &target);

inline int Spread_Distance(CoordStruct const &a, CoordStruct b)
{
    UnknownVector3 delta = UnknownVector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
    return delta.Length();
}

inline Vector3 Divide(Vector3 const &vector, double divisor)
{
    return Vector3(vector.X / divisor, vector.Y / divisor, vector.Z / divisor);
}

inline Vector3 Normalized(Vector3 const &v)
{
    double length = Fast_Sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
    if (length != 0.0) {
        return Vector3(v.X / length, v.Y / length, v.Z / length);
    }
    return v;
}

inline Cell Vector_Cell(Vector3 const &v)
{
    return Cell((short)v.X, (short)v.Y);
}

// Readable names for this row's constants.
enum {
    KIND_CELL = 0x0B,
    CELL_FLAG_TAKEN = 0x8000,
    CELL_FLAG_UNDER_BRIDGE = 0x100,
    SPREAD_TRIES = 6,
    SPREAD_KEY_SCALE = 1000,
    SPREAD_LEVEL_SLACK = 2,
};

void __fastcall Unknown_00629290(int start, int count)
{
    if (unknown_event_queue_a35f30[start].TargetAt(0x13).Kind == 0 || unknown_event_queue_a35f30[start].TargetAt(0x13).Kind != KIND_CELL) {
        return;
    }
    CellClass *cptr = unknown_event_queue_a35f30[start].TargetAt(0x13).As_Cell();
    if (cptr == 0 || cptr == &unknown_dummy_cell_a6f9f8) {
        return;
    }
    Cell dest = cptr->MapCoords;
    if (dest == unknown_geometry_k) {
        return;
    }
    DynamicVectorClass<ObjectClass *> list;
    int i;
    for (i = start; i < start + count; i++) {
        TechnoClass *techno = unknown_event_queue_a35f30[i].TargetAt(0x07).As_Techno();
        bool foot = false;
        if (techno != 0) {
            foot = techno->AbstractFlags.IsFoot;
        }
        if (foot && ((FootClass *)techno)->unknown_574 < 0) {
            list.Add(techno);
        }
    }
    if (list.Count() > 0) {
        CoordStruct center = Unknown_005D5010(list);
        ObjectClass *closest = Closest_Object_005d5090(list, center);
        IndexClass<int, ObjectClass *> index;
        for (i = 0; i < list.Count(); i++) {
            int key = Spread_Distance(list[i]->Center_Coord(), closest->Coord) * SPREAD_KEY_SCALE + i;
            index.Add_Index(key, list[i]);
        }
        list.Clear();
        DynamicVectorClass<Cell> cells;
        DynamicVectorClass<bool> done;
        for (i = 0; i < index.Count(); i++) {
            list.Add(index.Fetch_By_Position(i));
            cells.Add(dest);
            done.Add(false);
        }
        Cell origin = closest->unknown_slot104();
        for (i = 0; i < list.Count();) {
            ObjectClass *object = list[i];
            Cell best = dest;
            if (i == 0) {
                Map[dest].unknown_140 |= CELL_FLAG_TAKEN;
                done[0] = true;
            } else if (!done[i]) {
                Cell fallback = dest;
                Cell cell(object->unknown_slot19(0));
                int level = Map[dest].IsUnderBridge * 4 + Map[dest].unknown_11b;
                Vector3 position(dest.X + 0.5, dest.Y + 0.5, 0);
                int offset_x = cell.X - origin.X;
                int offset_y = cell.Y - origin.Y;
                Vector3 step(offset_x, offset_y, 0);
                step = Normalized(step);
                int tries = 0;
                bool detour = false;
                TechnoTypeClass *type = object->Techno_Type_Class();
                int zone = Map.Zone(dest, type->MovementZone, true);
                bool found = false;
                Cell spot;
                for (; tries < SPREAD_TRIES; tries++) {
                    position = step + position;
                    spot = Cell((short)position.X, (short)position.Y);
                    CellClass *spot_ptr = &Map[spot];
                    if (!Map.Unknown_0055A660(spot, true)) {
                        break;
                    }
                    int spot_zone = Map.Zone(spot, type->MovementZone, spot_ptr->IsUnderBridge);
                    int enter = object->Can_Enter_Cell(spot_ptr, -1, level, 0, 1);
                    if (spot_zone == zone) {
                        int flags = spot_ptr->unknown_140;
                        if (flags & CELL_FLAG_TAKEN) {
                            fallback = spot;
                        } else {
                            if ((flags & CELL_FLAG_UNDER_BRIDGE) && abs(level - spot_ptr->unknown_11b - 4) > SPREAD_LEVEL_SLACK) {
                                break;
                            }
                            if (!(flags & CELL_FLAG_UNDER_BRIDGE) && abs(level - spot_ptr->unknown_11b) > SPREAD_LEVEL_SLACK) {
                                break;
                            }
                            if (enter < 4 || enter == 6) {
                                if (detour) {
                                    int dx = abs(spot.X - dest.X);
                                    int dy = abs(spot.Y - dest.Y);
                                    int range = (dx > dy ? dx : dy) + 3;
                                    // The original stores range, then keeps the fallback either way: VC6 only
                                    // removes this test after the store has survived dead-store removal.
                                    if (Search.Test_Cell_Walk(spot, dest, (FootClass *)object,
                                            Map[spot].IsUnderBridge, Map[dest].IsUnderBridge, -1) > range && range > 0) {
                                        found = false;
                                    }
                                    break;
                                }
                                found = true;
                                break;
                            }
                            detour = true;
                        }
                    } else {
                        detour = true;
                    }
                }
                best = !found ? fallback : Cell((short)position.X, (short)position.Y);
                cells[i] = best;
                TargetClass target(&Map[best]);
                Unknown_00629220(start, count, TargetClass((AbstractClass *)object), target);
                Map[best].unknown_140 |= CELL_FLAG_TAKEN;
                done[i] = true;
            }
            for (int j = i + 1; j < list.Count(); j++) {
                if (!done[j] && object->unknown_slot104() == list[j]->unknown_slot104()) {
                    TargetClass target(&Map[best]);
                    Unknown_00629220(start, count, TargetClass((AbstractClass *)list[j]), target);
                    done[j] = true;
                }
            }
            i++;
        }
        for (i = 0; i < cells.Count(); i++) {
            Map[cells[i]].unknown_140 &= ~CELL_FLAG_TAKEN;
        }
    }
}


// void __fastcall Clean_DoList(ConnManClass *net) (0x00629E80).

void __fastcall Clean_DoList(ConnManClass *net)
{
    while (DoList.Count) {
        if (!DoList.First().IsExecuted && ::Frame <= DoList.First().Frame) {
            break;
        }
        DoList.Next();
    }
}


// void __fastcall Queue_Record() (0x00629ED0): this frame's unexecuted events to the record file.

void __fastcall Queue_Record()
{
    int i, j;
    if (::Frame == 0 && unknown_game_mode_a3d298 != 0) {
        return;
    }
    j = 0;
    for (i = 0; i < DoList.Count; i++) {
        if (::Frame == DoList[i].Frame && !DoList[i].IsExecuted) {
            j++;
        }
    }
    unknown_record_file_a3f59c.Write(&j, sizeof(j));
    for (i = 0; i < DoList.Count; i++) {
        if (::Frame == DoList[i].Frame && !DoList[i].IsExecuted) {
            unknown_record_file_a3f59c.Write(&DoList[i], sizeof(EventClass));
            j--;
        }
    }
}


// void __fastcall Compute_Game_CRC() (0x00629FA0; Red Alert's Compute_Game_CRC).
#include "shared/infantry.hpp"
#include "shared/unknown_list_832470.hpp"
#include "shared/winsock_interface.hpp"

// A coordinate folded into one CRC word: X / 10 low, Y / 10 shifted up 16.
inline int Coord_CRC(CoordStruct coord)
{
    return coord.X / 10 + (coord.Y / 10 << 16);
}

void __fastcall Compute_Game_CRC()
{
    int i, j;
    InfantryClass *infp;
    UnitClass *unitp;
    BuildingClass *bldgp;
    ObjectClass *objp;

    GameCRC = 0;

    for (i = 0; i < Infantry.Count(); i++) {
        infp = Infantry[i];
        unsigned long val = Coord_CRC(infp->Coord) + Dir_To_256(infp->unknown_2e0.Current());
        Add_CRC(&GameCRC, val);
    }

    for (i = 0; i < Units.Count(); i++) {
        unitp = Units[i];
        unsigned long val = Coord_CRC(unitp->Coord) + Dir_To_256(unitp->unknown_2e0.Current())
            + Dir_To_256(unitp->unknown_2f8.Current());
        Add_CRC(&GameCRC, val);
    }

    for (i = 0; i < unknown_BuildingClass_list_00a40af8.Count(); i++) {
        bldgp = unknown_BuildingClass_list_00a40af8[i];
        unsigned long val = Coord_CRC(bldgp->Coord) + Dir_To_256(bldgp->unknown_2e0.Current());
        Add_CRC(&GameCRC, val);
    }

    // The map layers, then the logic list; an anim (kind 4) with ID -2 is left out.
    for (i = 0; i < 5; i++) {
        for (j = 0; j < DisplayClass::Layer[i].Count(); j++) {
            objp = DisplayClass::Layer[i][j];
            if (objp->Kind_Of() != 4 || objp->Fetch_ID() != -2) {
                unsigned long val = Coord_CRC(objp->Coord) + objp->Kind_Of();
                Add_CRC(&GameCRC, val);
            }
        }
    }

    for (i = 0; i < unknown_list_832470.Count(); i++) {
        objp = unknown_list_832470[i];
        if (objp->Kind_Of() != 4 || objp->Fetch_ID() != -2) {
            unsigned long val = Coord_CRC(objp->Coord) + objp->Kind_Of();
            Add_CRC(&GameCRC, val);
        }
    }

    Add_CRC(&GameCRC, unknown_id_source_a3d290->RandomNumber());
    Unknown_0062CF40(::Frame % 256);
}


// void __fastcall Add_CRC(unsigned long *crc, unsigned long value) (0x0062A340; Red Alert's Add_CRC).

void __fastcall Add_CRC(unsigned long *crc, unsigned long value)
{
    int hibit;

    if ((*crc) & 0x80000000) {
        hibit = 1;
    } else {
        hibit = 0;
    }

    (*crc) <<= 1;
    (*crc) += value;
    (*crc) += hibit;
}


// void __fastcall Print_CRCs(EventClass *event) (0x0062A360; Red Alert's Print_CRCs): writes the game state
// to SYNC<player>.TXT (houses, every infantry, unit, building and aircraft by house, the map and logic layers,
// the event that failed and the CRC of every object list), echoing each section's CRC on debug page 4.
#include "shared/crt_stdio.hpp"
#include "shared/win_stub.hpp"
#include "shared/cpu_info.hpp"
#include "shared/network_statistics.hpp"
#include "shared/cell.hpp"
#include "shared/aircraft.hpp"
#include "shared/ccini_extra.hpp"
#include "shared/color_scheme.hpp"
#include "shared/target.hpp"
#include "shared/wwcrc_engine.hpp"
#include "shared/unit_type.hpp"
#include "shared/infantry_type.hpp"
#include "shared/building_type_list.hpp"
#include "shared/aircraft_type.hpp"
#include "shared/anim.hpp"
#include "shared/anim_type.hpp"
#include "shared/task_force.hpp"
#include "shared/team_type.hpp"
#include "shared/team.hpp"
#include "shared/script.hpp"
#include "shared/tag_type.hpp"
#include "shared/tag.hpp"
#include "shared/trigger_type.hpp"
#include "shared/trigger.hpp"
#include "shared/taction.hpp"
#include "shared/tevent.hpp"
#include "shared/factory_class.hpp"
#include "shared/voxel_anim_type.hpp"
#include "shared/voxel_anim.hpp"
#include "shared/warhead_type.hpp"
#include "shared/weapon_type.hpp"
#include "shared/particle.hpp"
#include "shared/particle_system.hpp"
#include "shared/bullet_type.hpp"
#include "shared/bullet.hpp"
#include "shared/waypoint_path.hpp"
#include "shared/smudge_type.hpp"
#include "shared/overlay_type.hpp"
#include "shared/light_source.hpp"
#include "shared/building_light.hpp"
#include "shared/tube.hpp"
#include "shared/side.hpp"
#include "shared/tiberium.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/vector_class.hpp"
#include "shared/abstract.hpp"
#include "shared/empulse_class.hpp"

// Canonical Mono page getter at 0x0059DA50; declared here to use the existing provider.
MonoClass *__fastcall Mono_Page_0059da50(int page);

#define PRINT_LIST_CRCS(title, list)                                                        \
    fprintf(file, title);                                                                   \
    fprintf(file, "Index     CRC\n");                                                       \
    fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");                       \
    delete crc;                                                                             \
    crc = new WWCRCEngine;                                                                  \
    for (i = 0; i < list.Count(); i++) {                                                    \
        list[i]->Compute_CRC(*crc);                                                         \
        fprintf(file, "%05d      %08x\n", i, *crc);                                          \
    }

// The kind name of a target, its sync ID in id.
inline char const *Sync_Target_Name(AbstractClass *target, int &id)
{
    bool is_object;
    char const *name;
    id = 0;
    name = "None";
    if (target) {
        is_object = target->AbstractFlags.IsTechno;
    }
    if (target) {
        if (is_object) {
            id = target->Fetch_Heap_ID();
        } else if (target->Kind_Of() == 11) {
            id = ((CellClass *)target)->MapCoords.Y << 16 | ((CellClass *)target)->MapCoords.X;
        }
        name = Name_From_RTTI((RTTIType)target->Kind_Of());
    }
    return name;
}

// The packed form of a target, -1 for none.
inline int Sync_Target(TargetClass const &target)
{
    if (!target.Is_Valid()) {
        return -1;
    }
    return target.As_Target();
}

void __fastcall Print_CRCs(EventClass *event)
{
    char filename[0x200];
    char time[0x80];
    char builder[0x80];
    char machine[0x80];
    int family;
    unsigned char has_mmx;
    int i, j;
    int house;

    sprintf(filename, "SYNC%01d.TXT", unknown_player_a35db4->ArrayIndex);
    Debug_Printf("Writing sync data to file %s\n", filename);
    MonoClass *mono = Mono_Page_0059da50(4);
    mono->Clear();
    mono->Set_Cursor(0, 0);
    _iobuf *file = fopen(filename, "wt");
    if (!file) {
        Debug_Printf("Error - failed to open sync file for writing. Error code %d - %s\n", GetLastError(), file);
        return;
    }

    fprintf(file, "RA2 Sync\n");
    fprintf(file, "\nVersion %s\n", Version_Name());
    fprintf(file, "Internal Version %s\n", VerNum.Version_Name());
    time[0] = 0;
    Build_Time_Text(time, sizeof(time));
    fprintf(file, "Release Build: %s by %s - %s\r\n", Unknown_00737ad0(machine, sizeof(machine)), Build_By_String_00737b00(builder, sizeof(builder)), time);
    family = 5;
    has_mmx = 0;
    Get_CPU_Info(&family, &has_mmx, 0, 0);
    fprintf(file, "CPU %s, MMX %s, Vendor: %s\r\n", Get_CPU_Type_Name(), has_mmx ? "Yes" : "No", unknown_cpu_text_8015a5);
    fprintf(file, "Frames: %d\n", ::Frame);
    fprintf(file, "Average FPS: %d\n", fps_sample_count_00a6eb24 ? accumulated_fps_00a6eb20 / fps_sample_count_00a6eb24 : 0);
    fprintf(file, "Max MaxAhead: %d\n", max_max_ahead_00a3d578);
    fprintf(file, "Latency setting: %d\n", unknown_a3fba4);
    fprintf(file, "Game speed setting: %d\n", unknown_a40b18);
    fprintf(file, "FrameSendRate: %d\n", unknown_frame_send_rate_a3d564);

    if (PacketTransport) {
        for (i = 0; i < PacketTransport->Get_Num_Local_Addresses(); i++) {
            unsigned int *address = (unsigned int *)PacketTransport->Get_Local_Address(i);
            if (address) {
                unsigned int ip = *address;
                fprintf(file, "Local address: %d.%d.%d.%d\n", ip & 255, (ip >> 8) & 255, (ip >> 16) & 255, ip >> 24);
            }
        }
    }

    for (i = 0; i < 8; i++) {
        UnknownNetworkPlayerStatistics &statistics = player_statistics_00a3d584[i];
        if (statistics.unknown_00[0]) {
            NetNumType net;
            NetNodeType node;
            statistics.Address.Get_Address(net, node);
            fprintf(file, "\nName: %s\n", statistics.unknown_00);
            fprintf(file, "Address: %d.%d.%d.%d\n", node[0], node[1], node[2], node[3]);
            fprintf(file, "Max avg round trip: %d\n", statistics.MaxAverageRoundTrip);
            fprintf(file, "Max round trip: %d\n", statistics.MaxRoundTrip);
            fprintf(file, "Resends: %d\n", statistics.Resends);
            fprintf(file, "Frame sync stalls: %d\n", statistics.FrameSyncStalls);
            fprintf(file, "Command cound stalls: %d\n", statistics.CommandCountStalls);
            fprintf(file, "Lost: %d\n", statistics.Lost);
            fprintf(file, "Percent lost: %d\n", statistics.PercentLost);
        }
    }

    for (i = 0; i < 256; i++) {
        fprintf(file, "CRC[%d]=%x\n", i, CRC[i]);
    }

    for (house = 0; house < Houses.Count(); house++) {
        GameCRC = 0;
        HouseClass *housep = Houses[house];
        fprintf(file, "%s: IsHuman:%d  Color:%s  ID:%d  HouseType:%s\n", (char const *)housep->IniName, housep->unknown_134,
            ColorSchemes[housep->Type->Color]->Name, housep->ArrayIndex, (char const *)HouseTypes[housep->Type->ArrayIndex]->ID);
        mono->Printf("House %s:%x\n", (char const *)housep->Type->ID, GameCRC);
    }

    for (house = 0; house < Houses.Count(); house++) {
        HouseClass *housep = Houses[house];
        GameCRC = 0;
        fprintf(file, "-------------------- %s Infantry -------------------\n", (char const *)housep->Type->ID);
        for (i = 0; i < Infantry.Count(); i++) {
            InfantryClass *infp = Infantry[i];
            if (infp->House == housep) {
                int target_id, navcom_id;
                char const *target_name;
                char const *navcom_name;
                unsigned long val = Coord_CRC(infp->Coord) + Dir_To_256(infp->unknown_2e0.Current());
                Add_CRC(&GameCRC, val);
                target_name = Sync_Target_Name(infp->unknown_228, target_id);
                navcom_name = Sync_Target_Name(infp->unknown_494, navcom_id);
                fprintf(file, "COORD:%x   Facing:%d   Mission:%d   Type:%d(%s)   Tgt:%s(%d) Speed:%d NavCom:%s(%d)\n",
                    Coord_CRC(infp->Coord), Dir_To_256(infp->unknown_2e0.Current()), infp->Get_Mission(),
                    infp->Type->ArrayIndex, (char const *)infp->Type->ID, target_name, target_id, int(infp->unknown_468 * 256.0),
                    navcom_name, navcom_id);
            }
        }
        mono->Printf("%s Infantry:%x\n", (char const *)housep->Type->ID, GameCRC);
    }

    for (house = 0; house < Houses.Count(); house++) {
        HouseClass *housep = Houses[house];
        GameCRC = 0;
        fprintf(file, "-------------------- %s Units -------------------\n", (char const *)housep->Type->ID);
        for (i = 0; i < Units.Count(); i++) {
            UnitClass *unitp = Units[i];
            if (unitp->House == housep) {
                int target_id, navcom_id;
                char const *target_name;
                char const *navcom_name;
                unsigned long val = Coord_CRC(unitp->Coord) + Dir_To_256(unitp->unknown_2e0.Current())
                    + Dir_To_256(unitp->unknown_2f8.Current());
                Add_CRC(&GameCRC, val);
                target_name = Sync_Target_Name(unitp->unknown_228, target_id);
                navcom_name = Sync_Target_Name(unitp->unknown_494, navcom_id);
                fprintf(file, "COORD:%x   Facing:%d   Facing2:%d   Mission:%d   Type:%d(%s)   Tgt:%s(%d)  NavCom:%s(%d)   TrkNum:%d TrkInd:%d SpdAc:%d\n",
                    Coord_CRC(unitp->Coord), Dir_To_256(unitp->unknown_2e0.Current()), Dir_To_256(unitp->unknown_2f8.Current()),
                    unitp->Get_Mission(), unitp->Type->ArrayIndex, (char const *)unitp->Type->ID, target_name, target_id,
                    navcom_name, navcom_id, unitp->Locomotor.Checked()->Get_Track_Number(),
                    unitp->Locomotor.Checked()->Get_Track_Index(), unitp->Locomotor.Checked()->Get_Speed_Accum());
            }
        }
        mono->Printf("%s Units:%x\n", (char const *)housep->Type->ID, GameCRC);
    }

    for (house = 0; house < Houses.Count(); house++) {
        HouseClass *housep = Houses[house];
        GameCRC = 0;
        fprintf(file, "-------------------- %s Buildings -------------------\n", (char const *)housep->Type->ID);
        for (i = 0; i < unknown_BuildingClass_list_00a40af8.Count(); i++) {
            BuildingClass *bldgp = unknown_BuildingClass_list_00a40af8[i];
            if (bldgp->House == housep) {
                unsigned long val = Coord_CRC(bldgp->Coord) + Dir_To_256(bldgp->unknown_2e0.Current());
                Add_CRC(&GameCRC, val);
                fprintf(file, "COORD:%x   Facing:%d   Mission:%d   Type:%d(%s)   Tgt:%x\n",
                    Coord_CRC(bldgp->Coord), Dir_To_256(bldgp->unknown_2e0.Current()), bldgp->Get_Mission(),
                    bldgp->Type->ArrayIndex, (char const *)bldgp->Type->ID, Sync_Target(TargetClass(bldgp)));
            }
        }
        mono->Printf("%s Buildings:%x\n", (char const *)housep->Type->ID, GameCRC);
    }

    for (house = 0; house < Houses.Count(); house++) {
        HouseClass *housep = Houses[house];
        GameCRC = 0;
        fprintf(file, "-------------------- %s Aircraft -------------------\n", (char const *)housep->Type->ID);
        for (i = 0; i < Aircraft.Count(); i++) {
            AircraftClass *aircraftp = Aircraft[i];
            if (aircraftp->House == housep) {
                unsigned long val = Coord_CRC(aircraftp->Coord) + Dir_To_256(aircraftp->unknown_2e0.Current());
                Add_CRC(&GameCRC, val);
                fprintf(file, "COORD:%x   Facing:%d   Mission:%d   Type:%d(%s)   Tgt:%x\n",
                    Coord_CRC(aircraftp->Coord), Dir_To_256(aircraftp->unknown_2e0.Current()), aircraftp->Get_Mission(),
                    aircraftp->Type->ArrayIndex, (char const *)aircraftp->Type->ID, Sync_Target(TargetClass(aircraftp)));
            }
        }
        mono->Printf("%s Buildings:%x\n", (char const *)housep->Type->ID, GameCRC);
    }

    GameCRC = 0;
    for (i = 0; i < 5; i++) {
        fprintf(file, ">>>> MAP LAYER %d <<<<\n", i);
        LayerClass &layer = DisplayClass::Layer[i];
        for (j = 0; j < layer.Count(); j++) {
            ObjectClass *objp = layer[j];
            if (objp->Kind_Of() == 4 && objp->Fetch_ID() == -2) continue;
            unsigned long val = Coord_CRC(objp->Coord) + objp->Kind_Of();
            Add_CRC(&GameCRC, val);
            fprintf(file, "Object %d: %x ", j, Coord_CRC(objp->Coord));
            if (objp->Kind_Of() == 0x2B || objp->Kind_Of() == 0x13) {
                fprintf(file, "%s\t(Type:%d) ", Name_From_RTTI((RTTIType)objp->Kind_Of()), 0);
            } else {
                fprintf(file, "%s\t(Type:%d(%s)) ", Name_From_RTTI((RTTIType)objp->Kind_Of()),
                    objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->ID);
            }
            HouseClass *owner = objp->Owning_House();
            if (owner) {
                fprintf(file, "Owner: %s\n", (char const *)owner->Type->ID);
            } else {
                fprintf(file, "Owner: NONE\n");
            }

        }
    }
    mono->Printf("Map Layers:%x  \n", GameCRC);

    GameCRC = 0;
    fprintf(file, ">>>> LOGIC LAYER <<<<\n");
    for (i = 0; i < unknown_list_832470.Count(); i++) {
        ObjectClass *objp = unknown_list_832470[i];
        if (objp->Kind_Of() == 4 && objp->Fetch_ID() == -2) continue;
        unsigned long val = Coord_CRC(objp->Coord) + objp->Kind_Of();
            Add_CRC(&GameCRC, val);
        fprintf(file, "Object %d: %x ", i, Coord_CRC(objp->Coord));
        if (objp->Kind_Of() == 2) {
            fprintf(file, "Aircraft  (Type:%d(%s)) ", objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->ID);
        } else if (objp->Kind_Of() == 4) {
            fprintf(file, "Anim      (Type:%d(%s)) ", objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->ID);
        } else if (objp->Kind_Of() == 6) {
            fprintf(file, "Building  (Type:%d(%s)) ", objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->ID);
        } else if (objp->Kind_Of() == 8) {
            fprintf(file, "Bullet    (Type:%d(%s)) ", objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->ID);
        } else if (objp->Kind_Of() == 15) {
            fprintf(file, "Infantry  (Type:%d(%s)) ", objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->ID);
        } else if (objp->Kind_Of() == 20) {
            fprintf(file, "Overlay   (Type:%d(%s)) ", objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->ID);
        } else if (objp->Kind_Of() == 29) {
            fprintf(file, "Smudge    (Type:%d(%s)) ", objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->ID);
        } else if (objp->Kind_Of() == 36) {
            fprintf(file, "Terrain   (Type:%d(%s)) ", objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->ID);
        } else if (objp->Kind_Of() == 1) {
            fprintf(file, "Unit      (Type:%d(%s)) ", objp->Class_Of()->Fetch_Heap_ID(), (char const *)objp->Class_Of()->ID);
        }
        HouseClass *owner = objp->Owning_House();
        if (owner) {
            fprintf(file, "Owner: %s\n", (char const *)owner->Type->ID);
        } else {
            fprintf(file, "Owner: NONE\n");
        }

    }
    mono->Printf("Logic:%x  \n", GameCRC);

    int random = unknown_id_source_a3d290->RandomNumber();
    mono->Printf("Random Number:%x  \n", random);
    fprintf(file, "\nRandom Number:%d\n", random);
    mono->Printf("My Frame:%d  \n", ::Frame);
    fprintf(file, "My Frame:%d\n", ::Frame);

    if (event) {
        fprintf(file, "Offending event:\n");
        fprintf(file, "Type:         %d\n", event->Type);
        fprintf(file, "Frame:        %d\n", event->Frame);
        fprintf(file, "ID:           %x\n", (char)event->House);
        fprintf(file, "CRC:          %x\n", event->LongAt(0x07));
        fprintf(file, "CommandCount: %d\n", (unsigned short)event->ShortAt(0x0B));
        fprintf(file, "Delay:        %d\n", event->ByteAt(0x0D));
    }

    WWCRCEngine *crc = new WWCRCEngine;
    fprintf(file, "\n\n*************** House Type CRCs**************\n\n");
    fprintf(file, "Index     CRC\n");
    fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    for (i = 0; i < HouseTypes.Count(); i++) {
        HouseTypes[i]->Compute_CRC(*crc);
        fprintf(file, "%05d      %08x\n", i, *crc);
    }
    PRINT_LIST_CRCS("\n\n*************** House CRCs*******************\n\n", Houses)
    PRINT_LIST_CRCS("\n\n*************** Unit Type CRCs**************\n\n", UnitTypes)
    PRINT_LIST_CRCS("\n\n*************** Unit CRCs**************\n\n", Units)
    PRINT_LIST_CRCS("\n\n*************** Infantry Type CRCs**************\n\n", InfantryTypes)
    PRINT_LIST_CRCS("\n\n*************** Infantry CRCs**************\n\n", Infantry)
    PRINT_LIST_CRCS("\n\n*************** BuildingType CRCs**************\n\n", BuildingTypes)
    PRINT_LIST_CRCS("\n\n*************** Building CRCs**************\n\n", unknown_BuildingClass_list_00a40af8)
    PRINT_LIST_CRCS("\n\n*************** Aircraft Type CRCs**************\n\n", AircraftTypes)
    PRINT_LIST_CRCS("\n\n*************** Aircraft CRCs**************\n\n", Aircraft)
    fprintf(file, "\n\n*************** Anim CRCs**************\n\n");
    fprintf(file, "Index     CRC\n");
    fprintf(file, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    delete crc;
    crc = new WWCRCEngine;
    for (i = 0; i < Anims.Count(); i++) {
        AnimClass *animp = Anims[i];
        if (animp->Fetch_ID() != -2) {
            animp->Compute_CRC(*crc);
            fprintf(file, "%05d      %08x\n", i, *crc);
        }
    }
    PRINT_LIST_CRCS("\n\n*************** Anim Type CRCs**************\n\n", AnimTypes)
    PRINT_LIST_CRCS("\n\n*************** Task Force CRCs**************\n\n", TaskForces)
    PRINT_LIST_CRCS("\n\n*************** Team Type CRCs**************\n\n", TeamTypes)
    PRINT_LIST_CRCS("\n\n*************** Team CRCs**************\n\n", Teams)
    PRINT_LIST_CRCS("\n\n*************** Script Type CRCs**************\n\n", ScriptTypes)
    PRINT_LIST_CRCS("\n\n*************** Script CRCs**************\n\n", Scripts)
    PRINT_LIST_CRCS("\n\n*************** Tag Type CRCs**************\n\n", TagTypes)
    PRINT_LIST_CRCS("\n\n*************** Tag CRCs**************\n\n", Tags)
    PRINT_LIST_CRCS("\n\n*************** Trigger Type CRCs**************\n\n", TriggerTypes)
    PRINT_LIST_CRCS("\n\n*************** Trigger CRCs**************\n\n", Triggers)
    PRINT_LIST_CRCS("\n\n*************** Action CRCs**************\n\n", unknown_TActionClass_list_00abfd88)
    PRINT_LIST_CRCS("\n\n*************** Event CRCs**************\n\n", unknown_TEventClass_list_00ac0808)
    PRINT_LIST_CRCS("\n\n*************** Factory CRCs**************\n\n", Factories)
    PRINT_LIST_CRCS("\n\n*************** Voxel Anim Type CRCs**************\n\n", VoxelAnimTypes)
    PRINT_LIST_CRCS("\n\n*************** Voxel Anim CRCs**************\n\n", unknown_VoxelAnimClass_list_008399d0)
    PRINT_LIST_CRCS("\n\n*************** Warhead CRCs**************\n\n", unknown_WarheadTypeClass_list_00839b00)
    PRINT_LIST_CRCS("\n\n*************** Weapon CRCs**************\n\n", unknown_WeaponTypeClass_list_00839b90)
    PRINT_LIST_CRCS("\n\n*************** Particle Type CRCs**************\n\n", ParticleTypes)
    PRINT_LIST_CRCS("\n\n*************** Particle CRCs**************\n\n", unknown_ParticleClass_list_00a35e28)
    PRINT_LIST_CRCS("\n\n*************** Particle System CRCs**************\n\n", ParticleSystems)
    PRINT_LIST_CRCS("\n\n*************** Particle System Type CRCs**************\n\n", ParticleSystemTypes)
    PRINT_LIST_CRCS("\n\n*************** Bullet Type's**************\n\n", BulletTypes)
    PRINT_LIST_CRCS("\n\n*************** Bullet CRCs**************\n\n", unknown_BulletClass_list_00a40ce8)
    PRINT_LIST_CRCS("\n\n*************** Waypoint Path CRCs**************\n\n", unknown_WaypointPathClass_list_00b25590)
    PRINT_LIST_CRCS("\n\n*************** Smudge Type CRCs**************\n\n", SmudgeTypes)
    PRINT_LIST_CRCS("\n\n*************** Overlay Type CRCs**************\n\n", OverlayTypes)
    PRINT_LIST_CRCS("\n\n*************** Light Source CRCs**************\n\n", unknown_LightSourceClass_list_00a6e818)
    PRINT_LIST_CRCS("\n\n*************** Building Light CRCs**************\n\n", BuildingLights)
    PRINT_LIST_CRCS("\n\n*************** Tube CRCs**************\n\n", unknown_TubeClass_list_008661c8)
    PRINT_LIST_CRCS("\n\n*************** Sides CRCs**************\n\n", Sides)
    PRINT_LIST_CRCS("\n\n*************** Tiberium CRCs**************\n\n", unknown_TiberiumClass_list_00ac0b30)
    PRINT_LIST_CRCS("\n\n*************** Em Pulse CRCs**************\n\n", EMPulseClass::Array)
    delete crc;
    fclose(file);
}


#include "shared/system_timer.hpp"
#include "shared/audio.hpp"
#include "shared/object.hpp"


// void __fastcall Unknown_0062CF40(int index) (0x0062CF40): saves the game state of the frame into sync
// state slot index (houses, every infantry, unit, aircraft and building, the map and logic layers) for
// Unknown_0062DBA0; without sync logging it only draws the random number.

void __fastcall Unknown_0062CF40(int index)
{
    if (!unknown_flag_ab62e8) {
        unknown_random_aabce0 = unknown_id_source_a3d290->RandomNumber();
        return;
    }

    UnknownSyncState *state = &unknown_sync_states_a780d8[index];
    state->unknown_000 = GameCRC;
    state->RandomNumber = unknown_id_source_a3d290->RandomNumber();
    state->Frame = ::Frame;

    int i, j;
    AbstractClass *target;
    int id;
    bool is_object;     // assigned only for a target (left unset otherwise, as the original keeps it in memory)
    char const *name;
    for (i = 0; i < 8; i++) {
        UnknownSyncHouse *sh = &state->Houses[i];
        sh->ID = -1;
        if (i < Houses.Count()) {
            sh->Name[0] = 0;
            for (j = 0; j < unknown_players_a3fa7c.Count(); j++) {
                if (unknown_players_a3fa7c[j]->ID == i) {
                    strncpy(sh->Name, Wide_To_Narrow_006fa120(unknown_players_a3fa7c[j]->Name), sizeof(sh->Name));
                }
            }
            if (sh->Name[0] == 0 && (_stricmp(Houses[i]->Type->ID, "Neutral") == 0 ||
                                     _stricmp(Houses[i]->Type->ID, "Special") == 0)) {
                strcpy(sh->Name, Houses[i]->Type->ID);
            }
            sh->IsHuman = Houses[i]->unknown_134;
            sh->Color[sizeof(sh->Color) - 1] = 0;
            strncpy(sh->Color, ColorSchemes[Houses[i]->ColorSchemeIndex]->Name, sizeof(sh->Color) - 1);
            sh->ID = i;
            sh->HouseType[sizeof(sh->HouseType) - 1] = 0;
            strncpy(sh->HouseType, HouseTypes[Houses[i]->Type->ArrayIndex]->ID, sizeof(sh->HouseType) - 1);
        }
    }

    int objects = Infantry.Count() + Units.Count() + Aircraft.Count() + unknown_BuildingClass_list_00a40af8.Count();
    int layered = unknown_list_832470.Count();
    for (i = 0; i < 5; i++) {
        layered += DisplayClass::Layer[i].Count();
    }
    if (!state->Buffer) {
        state->Buffer = new char[state->BufferSize];
    }
    int size = objects * sizeof(UnknownSyncObject) + layered * sizeof(UnknownSyncLayerObject);
    if (size + 0x400 > state->BufferSize) {
        delete[] state->Buffer;
        state->BufferSize = (size + 0xFFFF) & 0xFFFF0000;
        Debug_Printf("Adjusted allocation of log %d to %dKb\n", index, state->BufferSize >> 10);
        state->Buffer = new char[state->BufferSize];
    }

    UnknownSyncObject *obj = (UnknownSyncObject *)state->Buffer;
    int count = 0;
    state->Objects = obj;

    state->Counts[0] = Infantry.Count();
    for (count = 0; count < Infantry.Count(); count++) {
        InfantryClass *infp = Infantry[count];
        obj->Coord = Coord_CRC(infp->Coord);
        obj->Facing = Dir_To_256(infp->unknown_2e0.Current());
        obj->Mission = infp->Get_Mission();
        obj->Type = infp->Type->ArrayIndex;
        obj->Speed = infp->unknown_468 * 256.0;
        obj->Owner = infp->House->ArrayIndex;
        obj->TypeName[sizeof(obj->TypeName) - 1] = 0;
        strncpy(obj->TypeName, infp->Type->ID, sizeof(obj->TypeName) - 1);
        target = infp->unknown_494;
        id = 0;
        if (target) {
            is_object = target->AbstractFlags.IsTechno;
        }
        if (target) {
            if (is_object) {
                id = target->Fetch_Heap_ID();
            } else if (target->Kind_Of() == 11) {
                id = ((CellClass *)target)->MapCoords.Y << 16 | ((CellClass *)target)->MapCoords.X;
            }
            name = Name_From_RTTI((RTTIType)target->Kind_Of());
            obj->NavCom[sizeof(obj->NavCom) - 1] = 0;
            obj->NavComID = id;
            strncpy(obj->NavCom, name, sizeof(obj->NavCom) - 1);
        } else {
            obj->NavComID = 0;
            obj->NavCom[0] = 0;
        }
        target = infp->unknown_228;
        id = 0;
        if (target) {
            is_object = target->AbstractFlags.IsTechno;
        }
        if (target) {
            if (is_object) {
                id = target->Fetch_Heap_ID();
            } else if (target->Kind_Of() == 11) {
                id = ((CellClass *)target)->MapCoords.Y << 16 | ((CellClass *)target)->MapCoords.X;
            }
            name = Name_From_RTTI((RTTIType)target->Kind_Of());
            obj->Target[sizeof(obj->Target) - 1] = 0;
            obj->TargetID = id;
            strncpy(obj->Target, name, sizeof(obj->Target) - 1);
        } else {
            obj->TargetID = 0;
            obj->Target[0] = 0;
        }
        obj++;
    }

    state->Counts[1] = Units.Count();
    for (i = 0; i < Units.Count(); i++) {
        UnitClass *unitp = Units[i];
        obj->Coord = Coord_CRC(unitp->Coord);
        obj->Facing = Dir_To_256(unitp->unknown_2e0.Current());
        obj->Facing2 = Dir_To_256(unitp->unknown_2f8.Current());
        obj->Mission = unitp->Get_Mission();
        obj->Type = unitp->Type->ArrayIndex;
        obj->Speed = unitp->unknown_468 * 256.0;
        obj->Owner = unitp->House->ArrayIndex;
        obj->TrackNumber = unitp->Locomotor.Checked()->Get_Track_Number();
        obj->TrackIndex = unitp->Locomotor.Checked()->Get_Track_Index();
        obj->SpeedAccum = unitp->Locomotor.Checked()->Get_Speed_Accum();
        obj->TypeName[sizeof(obj->TypeName) - 1] = 0;
        strncpy(obj->TypeName, unitp->Type->ID, sizeof(obj->TypeName) - 1);
        target = unitp->unknown_494;
        id = 0;
        if (target) {
            is_object = target->AbstractFlags.IsTechno;
        }
        if (target) {
            if (is_object) {
                id = target->Fetch_Heap_ID();
            } else if (target->Kind_Of() == 11) {
                id = ((CellClass *)target)->MapCoords.Y << 16 | ((CellClass *)target)->MapCoords.X;
            }
            name = Name_From_RTTI((RTTIType)target->Kind_Of());
            obj->NavCom[sizeof(obj->NavCom) - 1] = 0;
            obj->NavComID = id;
            strncpy(obj->NavCom, name, sizeof(obj->NavCom) - 1);
        } else {
            obj->NavComID = 0;
            obj->NavCom[0] = 0;
        }
        target = unitp->unknown_228;
        id = 0;
        if (target) {
            is_object = target->AbstractFlags.IsTechno;
        }
        if (target) {
            if (is_object) {
                id = target->Fetch_Heap_ID();
            } else if (target->Kind_Of() == 11) {
                id = ((CellClass *)target)->MapCoords.Y << 16 | ((CellClass *)target)->MapCoords.X;
            }
            name = Name_From_RTTI((RTTIType)target->Kind_Of());
            obj->Target[sizeof(obj->Target) - 1] = 0;
            obj->TargetID = id;
            strncpy(obj->Target, name, sizeof(obj->Target) - 1);
        } else {
            obj->TargetID = 0;
            obj->Target[0] = 0;
        }
        obj++;
        count++;
    }

    state->Counts[2] = Aircraft.Count();
    for (i = 0; i < Aircraft.Count(); i++) {
        AircraftClass *aircraftp = Aircraft[i];
        obj->Coord = Coord_CRC(aircraftp->Coord);
        obj->Facing = Dir_To_256(aircraftp->unknown_2e0.Current());
        obj->Mission = aircraftp->Get_Mission();
        obj->Type = aircraftp->Type->ArrayIndex;
        obj->Speed = aircraftp->unknown_468 * 256.0;
        obj->Owner = aircraftp->House->ArrayIndex;
        obj->TypeName[sizeof(obj->TypeName) - 1] = 0;
        strncpy(obj->TypeName, aircraftp->Type->ID, sizeof(obj->TypeName) - 1);
        target = aircraftp->unknown_494;
        id = 0;
        if (target) {
            is_object = target->AbstractFlags.IsTechno;
        }
        if (target) {
            if (is_object) {
                id = target->Fetch_Heap_ID();
            } else if (target->Kind_Of() == 11) {
                id = ((CellClass *)target)->MapCoords.Y << 16 | ((CellClass *)target)->MapCoords.X;
            }
            name = Name_From_RTTI((RTTIType)target->Kind_Of());
            obj->NavCom[sizeof(obj->NavCom) - 1] = 0;
            obj->NavComID = id;
            strncpy(obj->NavCom, name, sizeof(obj->NavCom) - 1);
        } else {
            obj->NavComID = 0;
            obj->NavCom[0] = 0;
        }
        target = aircraftp->unknown_228;
        id = 0;
        if (target) {
            is_object = target->AbstractFlags.IsTechno;
        }
        if (target) {
            if (is_object) {
                id = target->Fetch_Heap_ID();
            } else if (target->Kind_Of() == 11) {
                id = ((CellClass *)target)->MapCoords.Y << 16 | ((CellClass *)target)->MapCoords.X;
            }
            name = Name_From_RTTI((RTTIType)target->Kind_Of());
            obj->Target[sizeof(obj->Target) - 1] = 0;
            obj->TargetID = id;
            strncpy(obj->Target, name, sizeof(obj->Target) - 1);
        } else {
            obj->TargetID = 0;
            obj->Target[0] = 0;
        }
        obj++;
        count++;
    }

    state->Counts[3] = unknown_BuildingClass_list_00a40af8.Count();
    for (i = 0; i < unknown_BuildingClass_list_00a40af8.Count(); i++) {
        BuildingClass *bldgp = unknown_BuildingClass_list_00a40af8[i];
        obj->Coord = Coord_CRC(bldgp->Coord);
        obj->Facing = Dir_To_256(bldgp->unknown_2e0.Current());
        obj->Mission = bldgp->Get_Mission();
        obj->Type = bldgp->Type->ArrayIndex;
        obj->Owner = bldgp->House->ArrayIndex;
        obj->NavComID = 0;
        obj->NavCom[0] = 0;
        obj->TypeName[sizeof(obj->TypeName) - 1] = 0;
        strncpy(obj->TypeName, bldgp->Type->ID, sizeof(obj->TypeName) - 1);
        target = bldgp->unknown_228;
        id = 0;
        if (target) {
            is_object = target->AbstractFlags.IsTechno;
        }
        if (target) {
            if (is_object) {
                id = target->Fetch_Heap_ID();
            } else if (target->Kind_Of() == 11) {
                id = ((CellClass *)target)->MapCoords.Y << 16 | ((CellClass *)target)->MapCoords.X;
            }
            name = Name_From_RTTI((RTTIType)target->Kind_Of());
            obj->Target[sizeof(obj->Target) - 1] = 0;
            obj->TargetID = id;
            strncpy(obj->Target, name, sizeof(obj->Target) - 1);
        } else {
            obj->TargetID = 0;
            obj->Target[0] = 0;
        }
        obj++;
        count++;
    }
    state->ObjectCount = count;

    UnknownSyncLayerObject *obj_layer = (UnknownSyncLayerObject *)obj;
    for (i = 0; i < 5; i++) {
        state->Layers[i] = obj_layer;
        count = 0;
        for (j = 0; j < DisplayClass::Layer[i].Count(); j++) {
            ObjectClass *objp = DisplayClass::Layer[i][j];
            if (objp->Kind_Of() == 4 && objp->Fetch_ID() == -2) {
                continue;
            }
            obj_layer->Value = Coord_CRC(objp->Coord);
            obj_layer->RTTI = objp->Kind_Of();
            if (objp->Kind_Of() != 0x2B && objp->Kind_Of() != 0x13) {
                obj_layer->Type = objp->Class_Of()->Fetch_Heap_ID();
                obj_layer->Name[sizeof(obj_layer->Name) - 1] = 0;
                strncpy(obj_layer->Name, objp->Class_Of()->ID, sizeof(obj_layer->Name) - 1);
            } else {
                obj_layer->Type = -1;
                obj_layer->Name[0] = 0;
            }
            HouseClass *owner = objp->Owning_House();
            if (owner) {
                obj_layer->Owner = owner->ArrayIndex;
            } else {
                obj_layer->Owner = -1;
            }
            obj_layer++;
            count++;
        }
        state->LayerCounts[i] = count;
    }

    state->Logic = obj_layer;
    count = 0;
    for (i = 0; i < unknown_list_832470.Count(); i++) {
        ObjectClass *objp = unknown_list_832470[i];
        if (objp->Kind_Of() == 4 && objp->Fetch_ID() == -2) {
            continue;
        }
        obj_layer->Value = Coord_CRC(objp->Coord);
        obj_layer->RTTI = objp->Kind_Of();
        if (objp->Kind_Of() != 0x2B && objp->Kind_Of() != 0x13) {
            obj_layer->Type = objp->Class_Of()->Fetch_Heap_ID();
            obj_layer->Name[sizeof(obj_layer->Name) - 1] = 0;
            strncpy(obj_layer->Name, objp->Class_Of()->ID, sizeof(obj_layer->Name) - 1);
        } else {
            obj_layer->Type = -1;
            obj_layer->Name[0] = 0;
        }
        HouseClass *owner = objp->Owning_House();
        if (owner) {
            obj_layer->Owner = owner->ArrayIndex;
        } else {
            obj_layer->Owner = -1;
        }
        obj_layer++;
        count++;
    }
    state->LogicCount = count;
}


// void __fastcall Unknown_0062DBA0(int index, EventClass *event) (0x0062DBA0): writes the saved game
// state of slot index to SYNC<player>_<index>.TXT, with the offending event when there is one.

void __fastcall Unknown_0062DBA0(int index, EventClass *event)
{
    UnknownSyncState *state = &unknown_sync_states_a780d8[index];
    char filename[0x200];
    char time[0x80];
    char builder[0x80];
    char machine[0x80];
    int family;
    unsigned char has_mmx;
    int players;
    int house;
    int i;
    UnknownSyncObject *objects;

    sprintf(filename, "SYNC%01d_%03d.TXT", unknown_player_a35db4->ArrayIndex, index);
    Debug_Printf("Writing sync data to file %s\n", filename);
    _iobuf *file = fopen(filename, "wt");
    if (!file) {
        Debug_Printf("Error - failed to open sync file for writing. Error code %d - %s\n", GetLastError(), file);
        return;
    }

    fprintf(file, "RA2 Sync\n");
    fprintf(file, "\nVersion %s\n", Version_Name());
    fprintf(file, "Internal Version %s\n", VerNum.Version_Name());
    time[0] = 0;
    Build_Time_Text(time, sizeof(time));
    fprintf(file, "Release Build: %s by %s - %s\r\n", Unknown_00737ad0(machine, sizeof(machine)), Build_By_String_00737b00(builder, sizeof(builder)), time);
    family = 5;
    has_mmx = 0;
    Get_CPU_Info(&family, &has_mmx, 0, 0);
    fprintf(file, "CPU %s, MMX %s, Vendor: %s\r\n", Get_CPU_Type_Name(), has_mmx ? "Yes" : "No", unknown_cpu_text_8015a5);
    fprintf(file, "Frames: %d\n", ::Frame);
    fprintf(file, "Average FPS: %d\n", fps_sample_count_00a6eb24 ? accumulated_fps_00a6eb20 / fps_sample_count_00a6eb24 : 0);
    fprintf(file, "Max MaxAhead: %d\n", max_max_ahead_00a3d578);
    fprintf(file, "Latency setting: %d\n", unknown_a3fba4);
    fprintf(file, "Game speed setting: %d\n", unknown_a40b18);
    fprintf(file, "FrameSendRate: %d\n", unknown_frame_send_rate_a3d564);
    fprintf(file, "\nFrame: %d\n", state->Frame);
    fprintf(file, "Slot: %d\n", index);

    if (PacketTransport) {
        for (i = 0; i < PacketTransport->Get_Num_Local_Addresses(); i++) {
            unsigned int *address = (unsigned int *)PacketTransport->Get_Local_Address(i);
            if (address) {
                unsigned int ip = *address;
                fprintf(file, "Local address: %d.%d.%d.%d\n", ip & 255, (ip >> 8) & 255, (ip >> 16) & 255, ip >> 24);
            }
        }
    }

    for (i = 0; i < 8; i++) {
        UnknownNetworkPlayerStatistics &statistics = player_statistics_00a3d584[i];
        if (statistics.unknown_00[0]) {
            NetNumType net;
            NetNodeType node;
            statistics.Address.Get_Address(net, node);
            fprintf(file, "\nName: %s\n", statistics.unknown_00);
            fprintf(file, "Address: %d.%d.%d.%d\n", node[0], node[1], node[2], node[3]);
            fprintf(file, "Max avg round trip: %d\n", statistics.MaxAverageRoundTrip);
            fprintf(file, "Max round trip: %d\n", statistics.MaxRoundTrip);
            fprintf(file, "Resends: %d\n", statistics.Resends);
            fprintf(file, "Frame sync stalls: %d\n", statistics.FrameSyncStalls);
            fprintf(file, "Command cound stalls: %d\n", statistics.CommandCountStalls);
            fprintf(file, "Lost: %d\n", statistics.Lost);
            fprintf(file, "Percent lost: %d\n", statistics.PercentLost);
        }
    }

    for (i = 0; i < 256; i++) {
        if (i == index) {
            fprintf(file, "CRC[%d]=%x  *****\n", i, CRC[i]);
        } else {
            fprintf(file, "CRC[%d]=%x\n", i, CRC[i]);
        }
    }

    players = 0;
    for (i = 0; i < 8; i++) {
        UnknownSyncHouse &h = state->Houses[i];
        if (h.ID != -1) {
            players = h.ID;
            fprintf(file, "%s: IsHuman:%d  Color:%s  ID:%d  HouseType:%s\n", h.Name, h.IsHuman, h.Color, h.ID, h.HouseType);
        }
    }

    objects = state->Objects;
    for (house = 0; house < players; house++) {
        if (state->Houses[house].ID != -1) {
            fprintf(file, "-------------------- %s Infantry -------------------\n", state->Houses[house].Name);
            UnknownSyncObject *o = objects;
            for (i = 0; i < state->Counts[0]; i++) {
                if (o->Owner == house) {
                    fprintf(file, "COORD:%x   Facing:%d   Mission:%d   Type:%d(%s)   Tgt:%s(%d) Speed:%d NavCom:%s(%d)\n",
                        o->Coord, o->Facing, o->Mission, o->Type, o->TypeName, o->Target, o->TargetID, o->Speed, o->NavCom, o->NavComID);
                }
                o++;
            }
        }
    }
    objects += state->Counts[0];
    for (house = 0; house < players; house++) {
        if (state->Houses[house].ID != -1) {
            fprintf(file, "-------------------- %s Units -------------------\n", state->Houses[house].Name);
            UnknownSyncObject *o = objects;
            for (i = 0; i < state->Counts[1]; i++) {
                if (o->Owner == house) {
                    fprintf(file, "COORD:%x   Facing:%d   Facing2:%d   Mission:%d   Type:%d(%s)   Tgt:%s(%d)  NavCom:%s(%d)   TrkNum:%d TrkInd:%d SpdAc:%d\n",
                        o->Coord, o->Facing, o->Facing2, o->Mission, o->Type, o->TypeName, o->Target, o->TargetID, o->NavCom, o->NavComID, o->TrackNumber, o->TrackIndex, o->SpeedAccum);
                }
                o++;
            }
        }
    }
    objects += state->Counts[1];
    for (house = 0; house < players; house++) {
        if (state->Houses[house].ID != -1) {
            fprintf(file, "-------------------- %s Aircraft -------------------\n", state->Houses[house].Name);
            UnknownSyncObject *o = objects;
            for (i = 0; i < state->Counts[2]; i++) {
                if (o->Owner == house) {
                    fprintf(file, "COORD:%x   Facing:%d   Mission:%d   Type:%d(%s)   Tgt:%s(%d)  NavCom:%s(%d)\n",
                        o->Coord, o->Facing, o->Mission, o->Type, o->TypeName, o->Target, o->TargetID, o->NavCom, o->NavComID);
                }
                o++;
            }
        }
    }
    objects += state->Counts[2];
    for (house = 0; house < players; house++) {
        if (state->Houses[house].ID != -1) {
            fprintf(file, "-------------------- %s Buildings -------------------\n", state->Houses[house].Name);
            UnknownSyncObject *o = objects;
            for (i = 0; i < state->Counts[3]; i++) {
                if (o->Owner == house) {
                    fprintf(file, "COORD:%x   Facing:%d   Mission:%d   Type:%d(%s)   Tgt:%s(%d)\n",
                        o->Coord, o->Facing, o->Mission, o->Type, o->TypeName, o->Target, o->TargetID);
                }
                o++;
            }
        }
    }

    for (int layer = 0; layer < 5; layer++) {
        fprintf(file, ">>>> MAP LAYER %d <<<<\n", layer);
        for (i = 0; i < state->LayerCounts[layer]; i++) {
            UnknownSyncLayerObject &o = state->Layers[layer][i];
            fprintf(file, "Object %d: %x ", i, o.Value);
            if (o.RTTI == 0x2B || o.RTTI == 0x13) {
                fprintf(file, "%s\t(Type:%d) ", Name_From_RTTI((RTTIType)o.RTTI), 0);
            } else {
                fprintf(file, "%s\t(Type:%d(%s)) ", Name_From_RTTI((RTTIType)o.RTTI), o.Type, o.Name);
            }
            if (o.Owner != -1) {
                fprintf(file, "Owner: %s\n", state->Houses[o.Owner].Name);
            } else {
                fprintf(file, "Owner: None\n");
            }
        }
    }

    fprintf(file, ">>>> LOGIC LAYER <<<<\n");
    for (i = 0; i < state->LogicCount; i++) {
        UnknownSyncLayerObject &o = state->Logic[i];
        fprintf(file, "Object %d: %x ", i, o.Value);
        if (o.RTTI == 0x2B || o.RTTI == 0x13) {
            fprintf(file, "%s\t(Type:%d) ", Name_From_RTTI((RTTIType)o.RTTI), 0);
        } else {
            fprintf(file, "%s\t(Type:%d(%s)) ", Name_From_RTTI((RTTIType)o.RTTI), o.Type, o.Name);
        }
        if (o.Owner != -1) {
            fprintf(file, "Owner: %s\n", state->Houses[o.Owner].Name);
        } else {
            fprintf(file, "Owner: None\n");
        }
    }

    fprintf(file, "\nRandom Number:%d\n", state->RandomNumber);
    if (event) {
        fprintf(file, "Offending event:\n");
        fprintf(file, "Type:         %d\n", event->Type);
        fprintf(file, "Frame:        %d\n", event->Frame);
        fprintf(file, "ID:           %x\n", (char)event->House);
        fprintf(file, "CRC:          %x\n", event->LongAt(0x07));
        fprintf(file, "CommandCount: %d\n", (unsigned short)event->ShortAt(0x0B));
        fprintf(file, "Delay:        %d\n", event->ByteAt(0x0D));
    }
    fclose(file);
}


// 0x0062e520: function not yet identified
//
// Body: 129 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_0062e520(byte *param_1)
// Callees with accepted source:
//   FUN_004068f0 -> src/recovered/004068f0.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0062e520 {

extern void  FUN_004068f0(int);
extern void  FUN_004068f0(int, int);
extern "C" unsigned int DAT_00a3d560;
extern "C" unsigned int DAT_00a3d564;
extern "C" unsigned int Frame_00a40d2c;
extern "C" void* PTR_s_EMPTY_007d65cc;
extern "C" char s_FrameSendRate__d_007ecf8c[];
extern "C" char s_Frame___d_007ecfcc[];
extern "C" char s_Frame__d_007ecfa0[];
extern "C" char s_ID___d_007ecfbc[];
extern "C" char s_MaxAhead__d_007ecfac[];
extern "C" char s_Packet_received_too_late__007ed018[];
extern "C" char s_Type___s_007ecfdc[];
extern "C" char s___________Event_data_____________007ecfec[];

void __fastcall FUN_0062e520(unsigned char *event_bytes)
{
    FUN_004068f0((int)(s_Packet_received_too_late__007ed018));
    FUN_004068f0((int)(s___________Event_data_____________007ecfec));
    FUN_004068f0((int)(s_Type___s_007ecfdc), (int)((&PTR_s_EMPTY_007d65cc)[*event_bytes]));
    FUN_004068f0((int)(s_Frame___d_007ecfcc), (int)(*(unsigned int *)(event_bytes + 3)));
    FUN_004068f0((int)(s_ID___d_007ecfbc), (int)((int)(char)event_bytes[2]));
    FUN_004068f0((int)(s_MaxAhead__d_007ecfac), (int)(DAT_00a3d560));
    FUN_004068f0((int)(s_Frame__d_007ecfa0), (int)(Frame_00a40d2c));
    FUN_004068f0((int)(s_FrameSendRate__d_007ecf8c), (int)(DAT_00a3d564));
    return;
}

}  // namespace seed_0062e520


// 0x0062e5b0: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_0062e5b0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0062e5b0 {

void  FUN_0062e5b0(void)
{
    return;
}

}  // namespace seed_0062e5b0


// void __fastcall Unknown_0062E5C0(FrameSyncStruct *their, int index) (0x0062E5C0): a player's frames behind.

void __fastcall Unknown_0062E5C0(FrameSyncStruct *their, int index)
{
    long frame;
    unsigned int delay;
    unsigned int half_latency;
    unsigned int half_response;
    int frame_delay;
    int behind;
    int now;

    if (unknown_game_mode_a3d298 != 4) {
        unknown_a3fb84[index] = 0;
        return;
    }
    frame = their[index].Frame;
    if (unknown_flag_a3fba8) {
        half_latency = unknown_a402dc / 2;
        half_response = (unsigned int)their[index].unknown_10 / 2;
        double ratio = (double)unknown_a3d568 / unknown_frame_send_rate_a3d564;
        float rate = (float)ratio;
        int frames = (int)(-60.0f / rate);
        frame_delay = frames / 2;
        delay = half_response - frame_delay + half_latency;
    } else {
        delay = unknown_ipx_manager_a40978.unknown_slot22(index) / 2;
    }
    now = ::Frame;
    unsigned int scaled = delay * unknown_a6eb1c;
    unsigned int ticks = scaled / 60;
    behind = now - ticks - frame;
    unknown_a3fb84[index] = behind;
}


// bool __stdcall Unknown_0062E670(EventClass event) (0x0062E670): stamps the frame and adds the event to OutList.

bool __stdcall Unknown_0062E670(EventClass event)
{
    event.Frame = ::Frame;
    if (OutList.Add(event)) return true;
    return false;
}


// void __fastcall Unknown_0062E6E0(EventClass *event) (0x0062E6E0): a local event at the head of DoList.

void __fastcall Unknown_0062E6E0(EventClass *event)
{
    long frame = ::Frame + 1;
    if (unknown_session_a3d2ac == 2) {
        frame = ((::Frame + 1 + (unknown_frame_send_rate_a3d564 - 1)) / unknown_frame_send_rate_a3d564) * unknown_frame_send_rate_a3d564;
    }
    event->Frame = frame;
    if (DoList.Count < 0x4000) {
        if (DoList.Head == 0) {
            DoList.Head = 0x3FFF;
        } else {
            DoList.Head--;
        }
        DoList.Array[DoList.Head] = *event;
        DoList.Timings[DoList.Head] = timeGetTime();
        DoList.Count++;
        return;
    }
    if (unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5) {
        if (unknown_game_mode_a3d298 == 4) {
            Unknown_0069A160();
        }
        unknown_counter_a3fabc++;
        WWMessageBox(L" ").Process(StringTable::Load_String("TXT_QUEUE_FULL", 0, "D:\\Ra2\\Queue.CPP", 8826),
                                   StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\Queue.CPP", 8826), 0, 0, false);
        unknown_counter_a3fabc--;
        unknown_a3d8c8 = 0;
        unknown_a3d8cc = 0;
        unknown_flag_a40958 = false;
        if (unknown_ab62dc) {
            Unknown_0059DA00();
        }
        if (unknown_game_mode_a3d298 == 4) {
            unknown_flag_abd500 = true;
            unknown_flag_a3d8d1 = true;
            unknown_flag_a3d8d2 = true;
            Unknown_0069A160();
            Unknown_00698D00(false);
        }
    }
}


// void __fastcall Send_Port_Unreachable() (0x0062E840): NET_PORT_UNREACHABLE to every router connection.

void __fastcall Send_Port_Unreachable()
{
    UnknownGlobalPacket packet;
    int start;
    int i;
    char *name;

    Debug_Printf("Sending NET_PORT_UNREACHABLE packets\n");
    if (unknown_game_mode_a3d298 != 4) {
        return;
    }
    Unknown_00623280();
    start = unknown_ipx_manager_a40978.Global_Num_Send();
    for (i = 0; i < unknown_OwnerTalkClass_00ab6d18.Connections.Count(); i++) {
        memset(&packet, 0, 0x2F);
        packet.Command = 0x2F;
        strcpy(packet.Name, Wide_To_Narrow_006fa120(unknown_session_a3d298.Players[0]->Name));
        name = unknown_OwnerTalkClass_00ab6d18.Name_From_Index(i);
        if (name && _stricmp(name, Wide_To_Narrow_006fa120(unknown_session_a3d298.Players[0]->Name))) {
            Debug_Printf("Sending NET_PORT_UNREACHABLE to %s\n", name);
            IPXAddressClass address = *unknown_OwnerTalkClass_00ab6d18.Unknown_0063FA00(name);
            unknown_ipx_manager_a40978.Send_Global_Message(&packet, 0x2F, 1, &address, unknown_OwnerTalkClass_00ab6d18.Unknown_0063FAF0(name), 0);
        }
    }
    CDTimerClass<SystemTimerClass> timer(300);
    while (unknown_ipx_manager_a40978.Global_Num_Send() > start && timer) {
        unknown_481fb0();
    }
    Unknown_00623260();
}


// Original0062E9E0, existing22-byte provider; source and semantic identity preserved.
// QueueClass<EventClass,128>::Next is genuinely emitted from Queue_AI_Normal.
// The member implementation lives in shared/event.hpp. No authored stand-in.


// DynamicVectorClass<UnknownGlobalPacket *>::Delete (index; 0x0062EA00): VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX in
// shared/queue.hpp emits it for the Wait_For_Players row of this unit (Session's received global packets).


// The object-distance index used by Queue.CPP's group destination assignment.
template<> bool IndexClass<int,ObjectClass *>::Increase_Table_Size(int amount)
{
    if (amount>=0) {
        NodeElement *table=new NodeElement[IndexSize+amount];
        if (table) {
            for (int index=0;index<IndexCount;++index) table[index]=IndexTable[index];
            delete[] IndexTable;
            IndexTable=table;
            IndexSize+=amount;
            Invalidate_Archive();
            return true;
        }
    }
    return false;
}


// 0x0062ead0: function not yet identified
//
// Body: 34 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: int FUN_0062ead0(int *param_1,int *param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// Adjusted from the Ghidra seed by permute_seeds.py (signedness, comparison order) to
// match the original; edit freely, this file is no longer regenerated.

namespace seed_0062ead0 {

int  FUN_0062ead0(int *left_value, int *right_value)
{
    if (*left_value == *right_value) {
        return 0;
    }
    return ((*left_value >= *right_value) - 1 & 0xfffffffe) + 1;
}

}  // namespace seed_0062ead0
