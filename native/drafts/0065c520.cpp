// NON-EXACT DRAFT 0x65C520 FUN_0065c520
// entry 0x65C520; exact-match score 96.97 (Scout1049 wfp_9697, rebased by Scout1218); plan "draft, verify"
// origin lane: Scout1049 (build/class-work/Scout1049/w/wfp_9697.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1061 (native/drafts.json). Not original bytes.
// bool __fastcall Wait_For_Players_0065c520() (0x0065C520)
// @ref ??_C@_0CN@LDAF@Carrier?5Detect?5is?5false?4?5Abortin@=007f1f3c
// @ref ??_C@_0DI@OCKB@Player?5?$CFs?5failed?5to?5make?5it?5into@=007f1f04
// @ref ?Am_I_Master@SessionClass@@QAE_NPAVHouseClass@@@Z=0066f150
// @ref ?Average_Progress@ProgressScreenClass@@QAENXZ=0061f8c0
// @ref ?Destroy_Connection_005bb5d0@@YIXHH@Z=005bb5d0
// @ref __real@8@3ffeffbe76c8b4395800=007a9220
// @ref __real@8@3ffeffdf3b645a1cb000=007a9228
// The multiplayer load wait (not in a normal or mode 5 game): services the game
// until the loading screen's mean progress reaches 0.9995, then reports 100
// (200 with the scenario's +0x223B flag) and returns true. Every 60 ticks
// (300 at first) the own progress is resent; when no progress was made for 3600
// ticks the players below 0.999 are dropped and it fails, as it does when the
// serial line loses its carrier. Name hypothesis.
#include "shared/id_source.hpp"
#include "shared/session_state.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/progress_screen.hpp"
#include "shared/system_timer.hpp"
#include "shared/cd_timer.hpp"
#include "shared/keyboard.hpp"
#include "shared/win_modem.hpp"
#include "shared/debug_printf.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/house.hpp"
#include "shared/crt_memory.hpp"

char *__fastcall Wide_To_Narrow_006fa120(unsigned short const *string);

bool __fastcall Wait_For_Players_0065c520()
{
    if (unknown_game_mode_a3d298 == 0 || unknown_game_mode_a3d298 == 5) {
        return true;
    }
    CDTimerClass<SystemTimerClass> timeout;
    CDTimerClass<SystemTimerClass> resend(300);
    timeout = 3600;
    double last = ProgressScreen.Average_Progress();
    for (;;) {
        WWKeyboard->Check();
        Service_Game_IO_00481fb0();
        if (unknown_game_mode_a3d298 == 1 && SerialPort != 0
            && !(SerialPort->Get_Modem_Status() & 0x80)) {
            Debug_Printf("Carrier Detect is false. Aborting load wait\n");
            return false;
        }
        if (timeout.Value() <= 0) {
            for (int i = 1, player = 1; i < unknown_players_a3fa7c.Count(); player++, i++) {
                if (ProgressScreen.Get_Progress(player) < 0.999) {
                    NodeNameType *entry = unknown_players_a3fa7c[i];
                    char *name = Wide_To_Narrow_006fa120(entry->Name);
                    Debug_Printf("Player %s failed to make it into the game. Dropping %s\n", name, Wide_To_Narrow_006fa120(entry->Name));
                    int house = unknown_players_a3fa7c[i]->ID;
                    Destroy_Connection_005bb5d0(house, 1);
                    HouseClass *hp = Houses[house];
                    if (unknown_session_a3d298.Am_I_Master(hp)) {
                        unknown_session_a3d298.MasterPlayerID = -1;
                        memset(unknown_session_a3d298.MasterPlayerName, 0, sizeof(unknown_session_a3d298.MasterPlayerName));
                        i--;
                    } else {
                        i--;
                    }
                }
            }
            return false;
        }
        double progress = ProgressScreen.Average_Progress();
        if (progress >= 0.9995) {
            unknown_session_a3d298.Loading_Progress_00672160(unknown_id_source_a3d290->unknown_223b ? 200 : 100);
            return true;
        }
        if (progress != last) {
            timeout = 3600;
            last = progress;
        }
        if (resend.Value() <= 0) {
            resend = 60;
            unknown_session_a3d298.Loading_Progress_00672160(unknown_id_source_a3d290->unknown_223b ? 200 : 100);
        }
    }
}
