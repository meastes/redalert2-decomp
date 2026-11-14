// Behaviour-written bodies for native M22 (Scout1177): leaves the coverage
// ranking found under reached callers. NOT EXACT: each follows its pass-6a
// decompile (or the CRT routine the address holds) against the shared headers.
#define SESSION_STATE_DECLARE_TRAP_OBJECT
#include "shared/session_state.hpp"
#include "shared/tactical.hpp"
#include "shared/coord.hpp"
#include "shared/map.hpp"
#include "shared/layer.hpp"
#include "shared/id_source.hpp"
#include "shared/fogged_object.hpp"
#include "shared/object_type.hpp"
#include "shared/win32_api.hpp"
#include "shared/ddraw_result.hpp"
#include "shared/unknown_globals.hpp"
#include <ctype.h>
#include <stdio.h>

namespace ra2port { void *original_data_pointer(uint32_t va); }

// 0x005E0DA0 (pass-6a decompile): the owner-draw control kinds (dialog
// template IDs) listed for 0x005E4780/0x005E9410.
namespace OwnerDraw {
bool __fastcall Is_Listed_Kind_5e0da0(int kind)
{
    switch (kind) {
    case 0x125: case 0xE7: case 0x116: case 0xC2: case 0xC9: case 0x11D: case 0x11C: case 0x114: case 700:
    case 0x113: case 0x109: case 0x10F: case 0x117: case 0xE6: case 0xF3: case 0xF4: case 0x10E:
        return true;
    }
    return false;
}
}

// 0x007837D3: the CRT's wcscmp (-1, 0 or 1). Reached by the dialog default procedure 0x005FFB00.
int Compare_Wide_007837d3(unsigned short const *first, unsigned short const *second)
{
    int difference;
    while ((difference = (int)*first - (int)*second) == 0 && *second != 0) {
        ++first;
        ++second;
    }
    return difference < 0 ? -1 : difference > 0 ? 1 : 0;
}

// 0x00784AA8: the CRT's isspace through _isctype(c, _SPACE) (0x0078BBB0).
int __cdecl Unknown_isspace_00784aa8(short character)
{
    return character >= 0 && character < 256 && isspace((unsigned char)character) ? 8 : 0;
}

// 0x00672140 (pass-6a decompile): whether the scenario file is "RandMap_Sed"
// (_stricmp 0x00781F20; the string at 0x007E16E8). Reached by 0x005C7570.
extern "C" int __cdecl _stricmp(char const *left, char const *right);
bool SessionClass::Unknown_00672140()
{
    return _stricmp(ScenarioFileName_658, "RandMap_Sed") == 0;
}

// 0x006715C0: does nothing (unknown_53fbd0 calls it each frame).
void SessionClass::Trap_Object_006715c0()
{
}

// 0x005D5F40 (pass-6a decompile): frees the pip and talk-bubble shapes that
// ObjectTypeClass::One_Time 0x005D5E00 loaded from loose files. Reached by WinMain's end.
extern void const *unknown_pipbrd_shapes_a72fe0;
extern void const *unknown_pips_shapes_a72fe4;
extern void const *unknown_pips2_shapes_a72fe8;
extern void const *unknown_talkbubl_shapes_a72fec;
extern bool unknown_pipbrd_owned_a72ff0;
extern bool unknown_pips_owned_a72ff1;
extern bool unknown_pips2_owned_a72ff2;
extern bool unknown_talkbubl_owned_a72ff3;
void Free_Memory_00781d3b(void *block);
void Unknown_005D5F40()
{
    if (unknown_pipbrd_owned_a72ff0) {
        Free_Memory_00781d3b((void *)unknown_pipbrd_shapes_a72fe0);
        unknown_pipbrd_owned_a72ff0 = false;
    }
    if (unknown_pips_owned_a72ff1) {
        Free_Memory_00781d3b((void *)unknown_pips_shapes_a72fe4);
        unknown_pips_owned_a72ff1 = false;
    }
    if (unknown_pips2_owned_a72ff2) {
        Free_Memory_00781d3b((void *)unknown_pips2_shapes_a72fe8);
        unknown_pips2_owned_a72ff2 = false;
    }
    if (unknown_talkbubl_owned_a72ff3) {
        Free_Memory_00781d3b((void *)unknown_talkbubl_shapes_a72fec);
        unknown_talkbubl_owned_a72ff3 = false;
    }
}

// 0x005EAC20: does nothing (WinMain).
void Unknown_005EAC20()
{
}

// 0x006F6730 / 0x006F6740: the two mouse shapes DisplayClass slot 46 picks.
int Unknown_006F6730() { return 0x47; }
int Unknown_006F6740() { return 0x16; }
int Unknown_006F6740b() { return 0x16; }      // the same address under a second name (draft 0049b0f0)

// 0x00495770 (pass-6a decompile): reports a DirectDraw result: a message box for
// an error (or the handler 0x008523EC when one is set), and one for success when
// asked. The texts are the original's (0x007D59A8, 0x007D599C, 0x007D59C4, 0x007D59F4).
extern void *unknown_window_b263d8;
void __fastcall unknown_00495770(HRESULT result, int show_success)
{
    if (result == 0) {
        if (show_success) {
            MessageBoxA((HWND)unknown_window_b263d8, (char const *)ra2port::original_data_pointer(0x007D59C4),
                        (char const *)ra2port::original_data_pointer(0x007D59F4), 0);
        }
        return;
    }
    if (direct_draw_error_handler_008523ec) {
        direct_draw_error_handler_008523ec(result);
        return;
    }
    char text[80];
    char const *format = (char const *)ra2port::original_data_pointer(0x007D59A8);
    snprintf(text, sizeof text, format ? format : "DDRAW.DLL Error code = %08X", (unsigned)result);
    MessageBoxA((HWND)unknown_window_b263d8, text, (char const *)ra2port::original_data_pointer(0x007D599C), 0x30);
}

// 0x0047E2B0: does nothing (TerrainClass slot 86).
void Cell::unknown_47e2b0()
{
}

// 0x0054F240: the hash of a key (bits 12-15 or'ed into bits 0-3). MapClass::One_Time.
int __fastcall Unknown_Hash_0054F240(unsigned const &key)
{
    return (key >> 12 & 0xF) | (key & 0xF);
}

// 0x0040CAF0 / 0x0040CB00 (LayerClass slots 8, 9): Clear() through its virtual (slot 3); nothing.
void LayerClass::unknown_slot8()
{
    Clear();
}

void LayerClass::unknown_slot9()
{
}


// 0x004C2040: the cell the fogged object stands on (its coordinate / 256, signed).
Cell FoggedObjectClass::Get_Cell() const
{
    return unknown_coord_cell(Coord);
}
