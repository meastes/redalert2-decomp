// Network (LAN) play for yr-native (lane YW): data the LAN path reads that the YR tree only declares
// and yr_data.py cannot load generically. Behaviour code for the native build.
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

namespace ra2port {
void *original_data_pointer(uint32_t va);
bool original_bytes(uint32_t va, void *out, size_t n);
}

// OriginalCheats (gamemd.exe 0x00825C28; init.cpp's CheatEntryStruct): four entries of a state flag's
// address, the code's text, a version suffix and whether it is allowed in multiplayer. Select_Game's
// Cheat_Disable clears the flags of the disallowed ones before every network game, so with the
// table zero (pointers and a bool: yr_data.py leaves it unloaded) the Network button faulted. The
// table is read from the user's gamemd.exe once the original data is in, its addresses made native
// pointers (the globals' storage, else the mapped image): a function in the deferred initialisers'
// list (defer_initializers.py; this object is not one of the units it rewrites), so it runs after the
// load, not while the program starts.
struct CheatEntryStruct {
    bool *State;
    const char *CheatString;
    const char *VersionSuffix;
    bool IsAllowedInMP;
};
CheatEntryStruct OriginalCheats[4];

namespace {
void load_original_cheats()
{
    for (int i = 0; i < 4; ++i) {
        uint32_t entry[4] = {};
        if (!ra2port::original_bytes(0x00825C28 + 16 * i, entry, sizeof(entry))) return;
        OriginalCheats[i].State = (bool *)ra2port::original_data_pointer(entry[0]);
        OriginalCheats[i].CheatString = (const char *)ra2port::original_data_pointer(entry[1]);
        OriginalCheats[i].VersionSuffix = (const char *)ra2port::original_data_pointer(entry[2]);
        OriginalCheats[i].IsAllowedInMP = (entry[3] & 0xFF) != 0;
    }
}
typedef void (*Initialiser)(void);
#if defined(__MINGW32__)
__attribute__((section(".yrini$m"), used)) Initialiser load_original_cheats_entry = load_original_cheats;
#else
__attribute__((section("__DATA,__yr_init_func"), used)) Initialiser load_original_cheats_entry = load_original_cheats;
#endif
}

// FUN_0077D940 and FUN_0077D970 (gamemd.exe, Westwood Online's unit, cut from the native build):
// whether the Internet game is a tournament (a ladder or a clan game). Both are false unless
// Session.Type is GAME_INTERNET, and then read Westwood Online's flags (0x00B76B24, 0x00B76B30),
// which only that unit sets. Without Westwood Online neither can be true; the network dialogs
// (FUN_005ED5A0, netshare.cpp; multiplayer.cpp's mode and map lists) ask on every LAN screen.
bool FUN_0077D940(void) { return false; }
bool FUN_0077D970(void) { return false; }

namespace ra2port {
char *host_path(const char *game_path, char *out, size_t n);
char *host_path_write(const char *game_path, char *out, size_t n);
}

// The installation's serial for the network game (lane YW2; fixups.json, Decrypt_Serial 0x005DC170).
// gamemd.exe reads it from the registry its installer wrote (HKLM\SOFTWARE\Westwood\Yuri's Revenge,
// "Serial": the 22 digits of the CD key), sends it with every join request and answer, and the LAN
// host refuses a player whose serial it already has (Get_Join_Responses, reason 7 TXT_SERIAL_DUP).
// The native game has no installer: every copy read the same empty serial and no guest could ever
// join. Without one in the registry, the installation keeps its own in its settings folder
// (YRSERIAL.TXT, 22 digits made once at random), so two copies, on one machine or two, differ and a
// copy keeps its serial from one start to the next, as an installed game does.
void Yr_Installation_Serial(char *serial)
{
    enum { DIGITS = 22 };
    char path[1024];
    char text[DIGITS + 2] = {};
    bool ok = false;
    if (FILE *f = fopen(ra2port::host_path("YRSERIAL.TXT", path, sizeof(path)), "rb")) {
        ok = fread(text, 1, DIGITS, f) == DIGITS;
        fclose(f);
        for (int i = 0; ok && i < DIGITS; ++i) ok = text[i] >= '0' && text[i] <= '9';
    }
    if (!ok) {
        // The time, the processor clock and this run's addresses (randomised per process), mixed
        // (splitmix64): copies started together on one machine still differ.
        uint64_t seed = (uint64_t)time(NULL) ^ ((uint64_t)clock() << 32) ^ (uint64_t)(uintptr_t)path ^
                        ((uint64_t)(uintptr_t)&Yr_Installation_Serial << 16);
        for (int i = 0; i < DIGITS; ++i) {
            uint64_t z = (seed += 0x9E3779B97F4A7C15ull);
            z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
            z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
            text[i] = (char)('0' + (z ^ (z >> 31)) % 10);
        }
        text[DIGITS] = 0;
        if (FILE *f = fopen(ra2port::host_path_write("YRSERIAL.TXT", path, sizeof(path)), "wb")) {
            fwrite(text, 1, DIGITS, f);
            fclose(f);
        }
    }
    memcpy(serial, text, DIGITS);
    serial[DIGITS] = 0;
}
