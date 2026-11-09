// Skirmish test harness: save and leave (README "Skirmish harness";
// RA2_NATIVE_SAVE_AT=<frame>[,<file>]). Test scaffolding, never game behaviour
// (Scout1125, native M15): at that Main_Loop frame the harness does what the
// in-game options menu's Save button ends in (Options_Menu_Proc 0x004DD8A0
// case 0x51F: LoadOptionsClass::Save_Dialog 0x0053B1B0, unrecovered, then
// Save_Game_With_Dialog 0x0053C760 -> Save_Game 0x006554A0): Save_Game with
// the file name (default SAVE0001.SAV) and a description; with
// RA2_NATIVE_SAVE_LEAVE=1 it then leaves the game as the options menu's Abort
// does (Main_Loop_End 0x0053F840's player_aborts path: Do_Abort 0x0065E590,
// then 0x0053F780), back to the main menu. Notes harness-save.
#include "shared/load_dlg.hpp"
#include "shared/main_loop.hpp"

#include <stdlib.h>

extern "C" {
void ra2port_progress(const char *milestone, const char *detail);
}

namespace {
void Append(char *to, char const *text)
{
    while (*to) ++to;
    while (*text) *to++ = *text++;
    *to = 0;
}
}

void Native_Harness_Save(int frame)
{
    static int at = -2;
    static char file[64] = "SAVE0001.SAV";
    if (at == -2) {
        at = -1;
        char const *env = getenv("RA2_NATIVE_SAVE_AT");
        if (env) {
            at = atoi(env);
            char const *comma = env;
            while (*comma && *comma != ',') ++comma;
            if (*comma == ',') {
                int n = 0;
                for (char const *p = comma + 1; *p && n < (int)sizeof file - 1; ++p) file[n++] = *p;
                file[n] = 0;
            }
        }
    }
    // RA2_NATIVE_FORCE_END=<frame>[,lose]: the end-of-game screens without
    // playing the game out (test scaffolding for the score dialog: sets the
    // flag Main_Loop_End reads, as the house AI does on a win or a defeat).
    static int end_at = -2;
    static bool end_lose = false;
    if (end_at == -2) {
        char const *env = getenv("RA2_NATIVE_FORCE_END");
        end_at = env ? atoi(env) : -1;
        if (env) for (char const *p = env; *p; ++p) if (*p == ',') end_lose = p[1] == 'l';
    }
    if (frame == end_at) {
        if (end_lose) player_loses_a40c78 = true; else player_wins_a35db1 = true;
    }
    if (at < 0 || frame != at) return;
    bool saved = Unknown_006554A0(file, L"Native M15 harness save", 0);
    char detail[128] = "frame ";
    char number[16];
    int n = 0, v = frame;
    do { number[n++] = '0' + v % 10; v /= 10; } while (v);
    char *d = detail;
    while (*d) ++d;
    while (n) *d++ = number[--n];
    *d = 0;
    Append(detail, saved ? ": saved " : ": save failed ");
    Append(detail, file);
    ra2port_progress("harness-save", detail);
    char const *leave = getenv("RA2_NATIVE_SAVE_LEAVE");
    if (saved && leave && *leave == '1') {
        player_aborts_a35db0 = true;
    }
}
