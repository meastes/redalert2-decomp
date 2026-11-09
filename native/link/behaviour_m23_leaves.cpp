// Behaviour rows for native M23 (Scout1187). NOT EXACT.
#include "shared/house.hpp"

// 0x004EEB90 (Leaf_4eeb90: an empty body). Reached by BuildingClass 0x0044D530.
void HouseClass::unknown_func_4eeb90()
{
}

// 0x00797ABA and 0x00797DFA (handwritten assembly; the other two pixel formats of
// LightConvertClass::Set_Tint 0x00538AA0's table builders, siblings of 0x0079777A /
// 0x00797440 in link/behaviour_launch.cpp; read from their decompiles): the same
// tinted table packed as (r & 0xF8) << 8 | (g & 0xF8) << 3 | b >> 2 (format 1) and
// (r & 0xFC) << 8 | (g & 0xF8) << 2 | b >> 3 (format 3). The plain path.
class PaletteClass;
namespace {
template <int RS, int RM, int GS, int GM, int BS>
void Build_Tint_Table(PaletteClass const *palette, unsigned short *table, int red, int green, int blue,
                      int level, unsigned char const *remap)
{
    unsigned char const *rgb = (unsigned char const *)palette;
    table[0] = 0;
    for (int i = 1; i < 256; ++i) {
        unsigned int r_scale = remap[i] ? red : level;
        unsigned int g_scale = remap[i] ? green : level;
        unsigned int b_scale = remap[i] ? blue : level;
        unsigned int r = ((unsigned int)rgb[3 * i] * r_scale) >> 16;
        unsigned int g = ((unsigned int)rgb[3 * i + 1] * g_scale) >> 16;
        unsigned int b = ((unsigned int)rgb[3 * i + 2] * b_scale) >> 16;
        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        table[i] = (unsigned short)((r & RM) << RS | (g & GM) << GS | b >> BS);
    }
}
}
void __cdecl FUN_00797aba(PaletteClass const *palette, unsigned short *table, int red, int green, int blue,
                          int level, unsigned char const *remap)
{
    Build_Tint_Table<8, 0xF8, 3, 0xF8, 2>(palette, table, red, green, blue, level, remap);
}
void __cdecl FUN_00797dfa(PaletteClass const *palette, unsigned short *table, int red, int green, int blue,
                          int level, unsigned char const *remap)
{
    Build_Tint_Table<8, 0xFC, 2, 0xF8, 3>(palette, table, red, green, blue, level, remap);
}

// 0x00795940 _com_issue_error (comsupp library: throws _com_error(result)). No source
// in the tree catches a _com_error, so in the original the throw ends the program
// (terminate); natively the HRESULT is reported and the program ends the same way.
// Reached only on a COM failure (111 callers ran: the smart-pointer and stream
// wrappers' error paths).
#include "shared/com_api.hpp"
#include <stdio.h>
#include <stdlib.h>
extern "C" void ra2port_trap_reached(const char *name);
void __stdcall _com_issue_error(HRESULT result)
{
    fprintf(stderr, "ra2-native: _com_issue_error 0x%08x (an uncaught _com_error ends the original too)\n", (unsigned)result);
    ra2port_trap_reached("_com_issue_error (uncaught _com_error)");
    abort();
}
