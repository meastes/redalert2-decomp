// The RA2 game's hooks the shared host and port call (native/platform/sdl/main.cpp,
// native/port/src/user32.cpp), for yr-native (lanes Y1, Y2). The YR globals' table is generated
// (native/yr/tools/yr_data.py); there are no RA2 self-tests here.
#include <stdint.h>
#include <mach-o/getsect.h>
#include <mach-o/ldsyms.h>
#include <stdio.h>
extern "C" {
// The game this process runs, for the port (original_image.cpp: gamemd.exe and its build check).
int ra2port_game_yr(void) { return 1; }
int ra2_native_draw_selftest(void) { return 2; }
int ra2_native_vqa_selftest(void) { return 2; }
// The launcher handshake (main.cpp: the native game is its own launcher). YR's names are the
// strings its CopyProtection code passes (IsLauncherRunning 0x0049F5C0, NotifyLauncher 0x0049F620,
// CheckProtectedData 0x0049F7A0); the symbols keep the RA2 host's names. `extern`: a const
// definition has internal linkage otherwise, and the link closure would make zero holes for them.
extern const char launch_mutex_name_007d515c[] = "48BC11BD-C4D7-466b-8A31-C6ABBAD47B3E";
}
extern const char launcher_event_007d51f0[] = "D6E7FC97-64F9-4d28-B52C-754EDF721C6F";
extern const char launcher_expected_message_007d52b8[] = "UIDATA,3DDATA,MAPS";
// After the port has filled the YR globals from gamemd.exe (main.cpp, before WinMain): the YR
// units' static initialisers, which defer_initializers.py took out of dyld's list, in link order
// (the units' order), as the original's _initterm ran them with its data in place.
void yr_construct_globals();   // yr_globals.cpp

void ra2_native_original_records()
{
    yr_construct_globals();
    unsigned long size = 0;
    uint8_t *start = getsectiondata(&_mh_execute_header, "__DATA", "__yr_init_func", &size);
    typedef void (*Initialiser)(void);
    Initialiser *list = (Initialiser *)start;
    unsigned long count = size / sizeof(Initialiser);
    for (unsigned long i = 0; i < count; ++i) list[i]();
    fprintf(stderr, "yr-native: %lu static initialisers run after the original data\n", count);
}
