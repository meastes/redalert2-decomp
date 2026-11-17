// bool __fastcall Find_Local_Scenario_0066c5b0(char *filename, int length, char *digest, bool official, bool unknown)
// (0x0066C5B0): looks the multiplayer map up by file name (never RandMap.Sed); with `unknown` set it asks for the
// disk the map is on when the file is missing; true (and the file name in the list's spelling) when the file
// has the given length and, unless digest is null or empty, its [Digest] 1 entry equals digest.
#include "shared/lobby.hpp"
#include "shared/session_state.hpp"
#include "shared/multi_mission.hpp"
#include "shared/cd.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/ini_class.hpp"
#include "shared/crt_string.hpp"

bool __fastcall Find_Local_Scenario_0066c5b0(char *filename, int length, char *digest, bool official, bool unknown)
{
    if (_stricmp(filename, "RandMap.Sed") != 0) {
        for (int index = 0; index < unknown_multi_missions_a3d8d8.Count(); index++) {
            if (_stricmp(unknown_multi_missions_a3d8d8[index]->Filename, filename) == 0) {
                CCFileClass file(unknown_multi_missions_a3d8d8[index]->Filename);
                if (!file.Is_Available() && unknown == true) {
                    MultiMission *mission = unknown_multi_missions_a3d8d8[index];
                    if (!mission->Is_On_CD(CD_Get_Current_Disk(60))) {
                        CD::Set_Required_CD(mission->On_Which_CD());
                    }
                    unknown_counter_a3fabc++;
                    if (!CD().Force_Available(CD::RequiredCD)) {
                        unknown_counter_a3fabc--;
                        return false;
                    }
                    unknown_counter_a3fabc--;
                }
                if (file.Is_Available() && file.Size() == length) {
                    INIClass ini;
                    ini.Load(file, false);
                    char file_digest[32];
                    ini.Get_String("Digest", "1", "No digest here mate. Nope.", file_digest, sizeof(file_digest));
                    if (digest == 0 || *digest == '\0' || strcmp(digest, file_digest) == 0) {
                        strcpy(filename, unknown_multi_missions_a3d8d8[index]->Filename);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
