// Behaviour rows (Scout1138, native M17; NOT EXACT, native/behaviour_rewrites.json):
// the key functions of the last two placeholder vtables (clang emits a class's
// vtable with its first non-inline virtual; until M17 both vtables were
// link-closure placeholders, so a call through them jumped through null slots).
#include "shared/wdt_map.hpp"
#include "shared/wdt_game_options.hpp"
#include "shared/wdt_territory.hpp"
#include "shared/string_table.hpp"
#include <stdint.h>

namespace ra2port {
void *original_data_pointer(uint32_t va);
bool original_bytes(uint32_t va, void *out, size_t n);
}

extern bool unknown_flag_7fbd04;    // 0x007FBD04 (slot 4 redraws the frame while set)

// 0x007291F0 (pass-6a decompile): sets the vtable and runs ~MSAnim; nothing of its own.
MSFrameAnim::~MSFrameAnim()
{
}

// 0x00729200 (pass-6a decompile; the same body is accepted as the free
// unknown_callback_6f3650, src/recovered/unowned): redraws the interface frame
// into the dirty area while 0x007FBD04 is set; never reports a change.
bool MSFrameAnim::Advance(Surface *surface, Rect *dirty)
{
    if (unknown_flag_7fbd04) {
        Unknown_006F3650(surface, *dirty, true);
    }
    return false;
}

namespace WorldDominationTour {

// The map size label (0x007AF140, four char pointers into .data, read from game.exe at run
// time: AGENTS.md "Native phase"; null when the image is not mapped).
static char const *Map_Size_Label(int index)
{
    uint32_t va = 0;
    if (index < 0 || index > 3 || !ra2port::original_bytes(0x007AF140u + 4u * (uint32_t)index, &va, 4)) {
        return 0;
    }
    return (char const *)ra2port::original_data_pointer(va);
}

// 0x00728B70 (pass-6a decompile): MapSize_Values (0x007FBB78, [4][4] ints) at the
// territory's +0x12 row and +0x13 column.
int MapSizeGameOption::Get_Value(rc_ptr<WDTTerritory> const &territory) const
{
    int const *values = (int const *)ra2port::original_data_pointer(0x007FBB78u);
    if (!values) {
        return 0;
    }
    return values[territory->unknown_12 * 4 + territory->unknown_13];
}

// 0x00728AB0 (pass-6a decompile; parked draft Scout777 77.48 not used): with a prefix,
// prefix + ", " + the CSF text of the +0x12 label (line 0x81); without one, the CSF text
// of the +0x13 label (line 0x83). The decompile reads the two bytes so; kept as read.
WideString MapSizeGameOption::Describe(rc_ptr<WDTTerritory> const &territory, WideString const &prefix) const
{
    if (prefix.Length() != 0) {
        WideString separator(L", ");
        return prefix + separator
            + StringTable::Load_String(Map_Size_Label(territory->unknown_12), 0, "D:\\Ra2\\WDTGameOptions.cpp", 0x81);
    }
    return WideString(StringTable::Load_String(Map_Size_Label(territory->unknown_13), 0, "D:\\Ra2\\WDTGameOptions.cpp", 0x83));
}

}
