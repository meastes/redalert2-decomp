// int __fastcall Map_Player_Count_005a2800(char const *filename) -- 0x005A2800
// The number of players of a map file: its [Waypoints] 0..7 that are set, else
// [RandomMap] NumPlayers; 0 without a file name or when the INI does not load.
#include "shared/multiplayer_game_mode.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/ini_class.hpp"
#include "shared/crt_string.hpp"

int __fastcall Map_Player_Count_005a2800(char const *filename)
{
    if (filename == 0) {
        return 0;
    }
    CCFileClass file(filename);
    INIClass ini;
    if (ini.Load(file, false) == 0) {
        return 0;
    }
    int count = 0;
    for (int i = 0; i < 8; i++) {
        char key[32];
        sprintf(key, "%d", i);
        if (ini.Get_Int("Waypoints", key, -1) != -1) {
            count++;
        }
    }
    if (count == 0) {
        count = ini.Get_Int("RandomMap", "NumPlayers", 0);
    }
    return count;
}
