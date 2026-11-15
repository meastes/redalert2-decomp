// The RA2 game's hooks the shared host and port call (native/platform/sdl/main.cpp,
// native/port/src/user32.cpp), for yr-native (lane Y1). Stage 1: the YR tree has no original-data
// table yet (its globals are zero until a YR loader reads them from gamemd.exe) and none of the
// RA2 game's records, self-tests or launcher names.
#include <stdint.h>
struct ra2_original_global { void *storage; uint32_t address; uint32_t extent; uint32_t shape; };
struct ra2_original_place { uint32_t address; uint32_t extent; void *storage; uint32_t pointers; };
extern "C" {
const ra2_original_global ra2_original_globals[1] = {{0, 0, 0, 0}};
const ra2_original_place ra2_original_storage[1] = {{0, 0, 0, 0}};
int ra2_native_draw_selftest(void) { return 2; }
int ra2_native_vqa_selftest(void) { return 2; }
// The launcher handshake names the RA2 game.exe stores (main.cpp: the app makes the mutex the
// launcher waits for); YR's are in gamemd.exe (stage 2 reads them).
char const launch_mutex_name_007d515c[] = "48BC11BD-C4D7-466b-8A31-C6ABBAD47B3E";
}
char const launcher_event_007d51f0[] = "";
char const launcher_expected_message_007d52b8[] = "";
void ra2_native_original_records() {}
