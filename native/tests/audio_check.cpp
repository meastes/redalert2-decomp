// Native proof (lane N30): the audio event library's list lookups built for arm64 from
// src/recovered through native_units.py, run here. Audio_Find_Listed 0x00404E00 hands
// Audio_Events_Update's sacrifice step the event type whose BucketNode sits first on
// Audio_Priority_Lists; the VC6 text subtracts BucketNode's VC6 offset (0xA4), so natively
// the type came back 0x24 bytes late and the step unlinked a garbage node (the user's iPhone
// crash in Hail to the Chief: a veteran crate's promotion sounds while the sample bank was
// full). port/fixups.json subtracts offsetof(AudioEventType, BucketNode) instead.
#define RA2_AUDIO_EVENT_SERVICE
#include "shared/audio.hpp"
#include <math.h>
#include <stdio.h>

// The lists are game data (data/); the port core counts the units' initialisers. This check
// links only the two units (CMakeLists.txt), so both are defined here.
AudioListNode Audio_Priority_Lists[7][10];
extern "C" int ra2port_initterm_mark(int) { return 0; }
// audevent.cpp's header statics (shared/unknown_geometry.hpp constants) call the fast math
// rows (the game aliases them to fast_math, link/address_wrappers.cpp); libm stand-ins here,
// as the constants are not used by this check.
double __cdecl unknown_sqrt_4ba410(double value) { return sqrt(value); }
double __cdecl unknown_math_4ba520(double value) { return sin(value); }
double __cdecl unknown_math_4ba5b0(double value) { return atan(value); }

static int failures = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main()
{
    static AudioEventType weak, strong;
    for (int row = 0; row < 7; ++row) {
        for (int column = 0; column < 10; ++column) {
            Audio_List_Init(&Audio_Priority_Lists[row][column]);
        }
    }
    CHECK(Audio_Find_Listed(7) == 0);

    // A type in row 2 (priority 2), volume step 5: found once the search covers row 2.
    Audio_List_Init_Node(&strong.BucketNode);
    Audio_List_Add_Tail(&Audio_Priority_Lists[2][5], &strong.BucketNode);
    CHECK(Audio_Find_Listed(2) == 0);
    CHECK(Audio_Find_Listed(3) == &strong);

    // A weaker type (row 1) is found first: the one the sacrifice step stops.
    Audio_List_Init_Node(&weak.BucketNode);
    Audio_List_Add_Tail(&Audio_Priority_Lists[1][0], &weak.BucketNode);
    AudioEventType *sacrifice = (AudioEventType *)Audio_Find_Listed(3);
    CHECK(sacrifice == &weak);

    // What Audio_Events_Update does with it: unlink the type's BucketNode.
    if (sacrifice == &weak) {
        Audio_List_Remove(&sacrifice->BucketNode);
    }
    CHECK(Audio_Find_Listed(3) == &strong);
    CHECK(Audio_Priority_Lists[1][0].Next == &Audio_Priority_Lists[1][0]);

    if (failures == 0) {
        printf("native-audio-check: ok\n");
    }
    return failures == 0 ? 0 : 1;
}
