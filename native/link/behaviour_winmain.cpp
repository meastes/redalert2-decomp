// Behaviour-written bodies for unmatched functions on WinMain's start-up path
// (0x0068E280 and what it calls before Main_Game). Each row has no accepted
// source and no compiling parked draft; the body follows the evidence named
// with it. NOT EXACT: every row stays unrecovered in the exact tree.
#include "shared/win32_api.hpp"

// 0x0077F0F4 set_control_word (Westwood math, TibSun code/cmathstuff.asm):
// stores the x87 control word (fstcw) into FPU_FLAG, which only the x87 ftol
// replacement reads. arm64 has no x87; the port keeps the control word as a
// value (_controlfp), so the native body records that value the same way.
extern "C" unsigned int _controlfp(unsigned int value, unsigned int mask);
static unsigned int fpu_flag_native;
void __cdecl set_control_word()
{
    fpu_flag_native = _controlfp(0, 0) & 0xFFFF;
}

// 0x00781F20 seen as a comparator (StringTable::Load_String 0x006F9EC0 bsearches
// the 0x28-byte label records with it): the address is the CRT's _stricmp, so
// the comparator is _stricmp on the two records' leading label text (the qsort
// in String_Table_Init 0x006F8E30 passes _stricmp itself).
extern "C" int _stricmp(char const *left, char const *right);
int __cdecl unknown_compare_781f20(void const *left, void const *right)
{
    return _stricmp((char const *)left, (char const *)right);
}

// 0x00496460: the video probe's debug log; empty in this build (the row's body
// is a bare return, src/recovered/00496460.cpp, like Debug_Printf 0x004068F0).
void __cdecl Video_Debug_Log_00496460(char const *format, ...) {}

// 0x004068F0 under its other declared name (Debug_Printf's address; empty in
// this build, link/behaviour_startup.cpp).
void __cdecl Debug_Print_004068f0(char const *format, ...) {}
// ... and its third (Read_Game_Modes' declaration; Scout1076).
void __cdecl Unknown_004068F0(char const *format, ...) {}

// 0x00406F70 (shared/debug_printf.hpp: the focus nesting counter's release;
// the row's seed, src/recovered/00406f70.cpp): counts 0x00831440 down and, on
// reaching zero, restarts the primary sound buffer (0x0040A870, accepted as
// Audio_DS_Restart_Primary). Its pair 0x00406F50 counts up and stops it.
extern int DAT_00831440;
void Audio_DS_Restart_Primary();
void Unknown_00406F70()
{
    if (DAT_00831440 != 0 && --DAT_00831440 == 0) {
        Audio_DS_Restart_Primary();
    }
}

// 0x00406F50, the pair (seed src/recovered/00406f50.cpp): the first holder stops
// the primary sound buffer (0x0040A860 Audio_DS_Stop_Primary), then counts up.
// Scout1111: reached at frame ~2,100 of the harness build order (a focus change).
void Audio_DS_Stop_Primary();
void Unknown_00406F50()
{
    if (DAT_00831440 == 0) {
        Audio_DS_Stop_Primary();
    }
    DAT_00831440 = DAT_00831440 + 1;
}

// 0x00735B10 (shared/unknown_globals.hpp: the current dialog window): the row's
// leaf returns the dword at 0x00B25DD4, the tree's CurrentDialog.
extern HWND CurrentDialog;
HWND __fastcall Unknown_00735B10()
{
    return CurrentDialog;
}

// 0x00522E80 (shared/wwmouse.hpp, keyboard.hpp: "also read by 0x00522E80"): the
// row's leaf returns the dword at 0x00A522AC, the recorded-input override.
extern int unknown_key_state_override_a522ac;
int unknown_function_522e80()
{
    return unknown_key_state_override_a522ac;
}

// Link adapter (one address, two declarations): Main_Game declares Init_Game
// 0x005116A0 as __fastcall (argc, argv); the accepted body is declared without
// parameters (its argc/argv arrive in ECX/EDX and are unused). Natively the two
// are different symbols, so the declared form forwards to the body.
int __fastcall Init_Game();
int __fastcall Init_Game(int argc, char *argv[])
{
    return Init_Game();
}

// 0x00496460 under its third declared name (shared/debug_printf.hpp: the
// loading-progress message; empty in this build, as Video_Debug_Log above).
void __cdecl Unknown_00496460(char const *format, ...) {}

// 0x005EAC10: an empty function in this build (the row's leaf is a bare
// return, src/recovered/005eac10.cpp); Init_Game calls it.
void Unknown_005EAC10() {}

// The accepted WinMain 0x0068E280 (b10418049) names two of the calls above by
// other declarations: 0x00785049 is the CRT's _controlfp (library code; its
// result is unused there) and 0x0077F0F4 is set_control_word. Natively the
// names are different symbols, so each forwards to the one body (Scout1153, M19).
void Set_FPU_Control(unsigned value, unsigned mask)
{
    _controlfp(value, mask);
}
void Unknown_0077F0F4()
{
    set_control_word();
}
