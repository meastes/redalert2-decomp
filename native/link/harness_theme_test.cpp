// RA2_NATIVE_THEME_TEST=1 (Scout1236, test scaffolding): at frame 10 of any game, the Sound
// options list's reads for every theme, as Sound_Option_Dialog_Func 0x00688DA0 makes them
// (unknown_6e8440, unknown_theme_length 0x006E8150, Full_Name 0x006E7CB0), printed to stderr
// as `theme-test <index> <flag> <length s> <name>`: the accessors' VC6-offset views over the
// arm64 ThemeClass faulted there (user report: SIGSEGV in unknown_theme_length+0x14).
#define THEME_SOUND_CONTROLS
#define THEME_CONTROL_RECORD
#include "shared/theme.hpp"
#include <stdio.h>
#include <stdlib.h>

void Native_Harness_Theme_Test(int frame)
{
    if (frame != 10 || !getenv("RA2_NATIVE_THEME_TEST")) return;
    int count = Theme.Max_Themes();
    for (int index = 0; index < count; ++index) {
        bool flag = Theme.unknown_6e8440((unsigned)index);
        int length = Theme.unknown_theme_length((unsigned)index);
        unsigned short const *name = Theme.Full_Name((unsigned)index);
        char text[80];
        int n = 0;
        for (; name && name[n] && n < 79; ++n) text[n] = (char)name[n];
        text[n] = 0;
        fprintf(stderr, "theme-test %d %d %d:%02d %s\n", index, flag ? 1 : 0, length / 60, length % 60, text);
    }
    fprintf(stderr, "theme-test: %d themes\n", count);
}
