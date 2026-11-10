// Behaviour-written bodies for unmatched functions the start-up and exit paths
// reach (static initialisers and their atexit destructors). Each row has no
// accepted source and no compiling parked draft; the body follows the evidence
// named with it (the row's seed/decompile, its header's comment, TibSun).
// NOT EXACT: every row stays unrecovered in the exact tree.
#include "shared/debug_printf.hpp"
#include "shared/win32_api.hpp"
#include "shared/cpu_info.hpp"

// 0x004068F0: empty in this build (the header says so; the row's body is a
// bare return, src/recovered/004068f0.cpp).
// Native diagnostic (test runs only): RA2_NATIVE_DEBUG_LOG=1 prints the
// game's debug lines to stderr; the original's body is empty.
extern "C" char *getenv(char const *);
extern "C" int dprintf(int, char const *, ...);
// The port's VC6-semantics formatter (port/src/crt.cpp): %ls/%S take the game's 16-bit wide strings
// (the host vsnprintf reads 32-bit wchar_t and dropped every line that printed a wide name).
extern "C" int ra2__vsnprintf(char *dst, unsigned long cap, char const *fmt, __builtin_va_list ap);
void __cdecl Debug_Printf(char const *format, ...)
{
    static int enabled = -1;
    if (enabled < 0) {
        char const *value = getenv("RA2_NATIVE_DEBUG_LOG");
        enabled = value != 0 && value[0] == '1';
    }
    if (enabled && format != 0) {
        char line[2048];
        __builtin_va_list args;
        __builtin_va_start(args, format);
        int n = ra2__vsnprintf(line, sizeof(line) - 1, format, args);
        __builtin_va_end(args);
        line[n < 0 ? sizeof(line) - 1 : n] = 0;   // VC6: -1 and no terminator when cut
        dprintf(2, "%s", line);
    }
}

// 0x00613E00 (__fastcall): makes the notice sink the planning mode's sink; the
// row stores ECX to 0x00A76680 (src/recovered/00613e00.cpp).
class INoticeSink;
struct UnknownPlanningSink;
extern UnknownPlanningSink *unknown_sink_a76680;
void __fastcall Unknown_Set_Sink_00613e00(INoticeSink *sink)
{
    unknown_sink_a76680 = (UnknownPlanningSink *)sink;
}

// 0x005ABF20 (shared/cpu_info.hpp; the row's seed): the performance counter
// frequency, low dword returned and high dword stored, both 0 on failure.
unsigned int __fastcall Performance_Frequency_005abf20(unsigned int *high)
{
    LARGE_INTEGER frequency;
    if (QueryPerformanceFrequency(&frequency)) {
        *high = frequency.HighPart;
        return frequency.LowPart;
    }
    *high = 0;
    return 0;
}

// 0x00711C70 (exit): frees the veinhole buffer (operator delete 0x00781D3B) and
// clears the pointer (the row's seed, src/recovered/00711c70.cpp).
extern unsigned char *unknown_veinhole_buffer_ad07a0;
void Unknown_00711C70()
{
    if (unknown_veinhole_buffer_ad07a0 != 0) {
        operator delete(unknown_veinhole_buffer_ad07a0);
        unknown_veinhole_buffer_ad07a0 = 0;
    }
}

// 0x006FA030 (static, shared/string_table.hpp): whether the string table is
// loaded: the row returns the dword at 0x00AD044C (src/recovered/006fa030.cpp).
extern int unknown_initialized_ad044c;
class StringTable {
public:
    static int Is_Loaded_006fa030();
};
int StringTable::Is_Loaded_006fa030() { return unknown_initialized_ad044c; }
