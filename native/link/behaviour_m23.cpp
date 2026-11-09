// Behaviour rows and port helpers for native M23 (Scout1187). NOT EXACT: nothing here
// is original code; every row stays unrecovered in the exact tree.
#include "shared/ww_message_box.hpp"
#include "shared/string_table.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/options.hpp"
#include "shared/audio.hpp"
#define RA2_INIT_SELECT_GAME   // the cheat record layout (Init.CPP unit macro)
#include "shared/init_cheat_codes.hpp"
#include <stdint.h>
#include <stdio.h>
extern "C" void *malloc_zone_from_ptr(const void *ptr);   // libmalloc (malloc/malloc.h does not take the prelude)

void Free_Memory_00781d3b(void *block);   // link/behaviour_m22_options.cpp
extern "C" void ra2port_trap_reached(const char *name);

// The seed-generated sources' CRT free 0x00781D3B (native_units.py rewrites their
// `FUN_00781d3b((undefined4)(x))` to a pointer-sized call here). A pointer the seed
// held at full width (`this` in the 74 scalar deleting destructors, pointer locals)
// is freed as the original does. A value the seed read through a 32-bit field or a
// 4-byte global is a cut address on arm64 (no heap block lives below 4 GiB): it is
// reported as a trap and not freed, as freeing it would only corrupt the heap.
extern "C" void ra2port_seed_free(intptr_t block)
{
    if (block == 0) {
        return;
    }
    if ((uintptr_t)block <= 0xFFFFFFFFu || (intptr_t)(int32_t)block == block
        || malloc_zone_from_ptr((void *)block) == 0) {
        static int reported;
        if (reported++ < 8) {
            fprintf(stderr, "ra2-native: seed free of a cut pointer 0x%llx\n", (unsigned long long)block);
        }
        ra2port_trap_reached("seed free 0x00781D3B of a 32-bit value");
        return;
    }
    Free_Memory_00781d3b((void *)block);
}

// Native single player scope (user decision: multiplayer is out of scope; WOL, IPX
// and modem code stay link-closure traps). Select_Game 0x005135F0 asks this before
// its Internet (WOL) and Network (LAN) selections (port/fixups.json): a message box
// says so and the main menu comes back, instead of faulting in IPXManagerClass.
bool Native_Multiplayer_Unavailable()
{
    unsigned short const *ok = StringTable::Load_String("GUI:Ok", 0, "native", 0);
    unknown_5159b0(0, true);
    WWMessageBox((unsigned short const *)L" ").Process(
        (unsigned short const *)L"Multiplayer is not available in this build.",
        ok, 0, 0, false);
    unknown_5159b0(0, true);
    return true;
}

// Audio state the original keeps in its initialised data as zeros (0x00830ED8,
// 0x00830F70, 0x00830FD8: all zero in game.exe): native storage, zero as there.
// Audio_Events_Init 0x00403EB0 / Audio_Init 0x00406AD0 initialise them.
AudioLevelSet Audio_Event_Levels;           // 0x00830ED8
AudioListNode Audio_Event_Type_List;        // 0x00830F70
AudioLevelSet Audio_Level_Set_830fd8;       // 0x00830FD8

namespace ra2port {
bool original_bytes(uint32_t va, void *out, size_t n);
void *original_data_pointer(uint32_t va);
}

// The main menu's four cheat records at 0x007DB500 (flag, code, second text, allowed in
// multiplayer; 16 bytes each) and the build time stamp at 0x007D8940 (a FILETIME), read
// from game.exe after the original data (platform/sdl/main.cpp). With the link closure's
// zero placeholder the main menu's key handler 0x00516F00 called strstr(text, 0) on the
// first letter typed and faulted. The flags are the native globals at those addresses;
// 0x00A40D38 has no recovered reader and gets native storage.
UnknownCheatCode unknown_cheat_codes_7db500[4];
alignas(8) unsigned char ra2_build_time_7d8940[8] __asm__("_unknown_build_time_7d8940");
namespace {
bool unknown_cheat_flag_a40d38;             // 0x00A40D38 (no recovered reader)
}
void ra2_native_original_records()
{
    struct { uint32_t address; bool *flag; } const flags[] = {
        {0x00A40D38u, &unknown_cheat_flag_a40d38},
        {0x00A40D39u, &unknown_a40d39},
        {0x00A40B41u, &Options.AllowHiResModes},
        {0x00A40D0Bu, &unknown_allow_mode_toggle_a40d0b},
    };
    for (int i = 0; i < 4; ++i) {
        uint32_t record[4] = {0, 0, 0, 0};
        if (!ra2port::original_bytes(0x007DB500u + 16 * i, record, sizeof record)) {
            return;
        }
        UnknownCheatCode &code = unknown_cheat_codes_7db500[i];
        code.Flag = 0;
        for (auto const &f : flags) {
            if (f.address == record[0]) code.Flag = f.flag;
        }
        if (code.Flag == 0) code.Flag = (bool *)ra2port::original_data_pointer(record[0]);
        if (code.Flag == 0) code.Flag = &unknown_cheat_flag_a40d38;
        code.Code = (char const *)ra2port::original_data_pointer(record[1]);
        code.unknown_8 = (char const *)ra2port::original_data_pointer(record[2]);
        code.AllowedInMultiplayer = (record[3] & 0xFF) != 0;
        if (code.Code == 0) code.Code = "\x01";   // never typed (toupper'd letters only)
    }
    ra2port::original_bytes(0x007D8940u, ra2_build_time_7d8940, sizeof ra2_build_time_7d8940);
}
