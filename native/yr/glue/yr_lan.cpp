// Network (LAN) play for yr-native (lane YW): data the LAN path reads that the YR tree only declares
// and yr_data.py cannot load generically. Behaviour code for the native build.
#include <stdint.h>
#include <stddef.h>
#include <string.h>

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
