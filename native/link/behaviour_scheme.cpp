// Behaviour-written body of an unmatched row on the skirmish launch path
// (Scout1081; native/behaviour_rewrites.json). NOT EXACT: the row stays
// unrecovered in the exact tree; never original code or bytes.
#include "shared/scheme_table.hpp"
#include "shared/color_scheme.hpp"

// 0x00603D50 Unknown_00603D50 (palette_name.hpp: the extra building palettes
// loaded so far; TechnoTypeClass::Read_INI allows one). The row's seed makes
// the scheme table on first use (SchemeTable's constructor expanded), then
// walks the table's keys as UnknownSchemeListIterator::Next (0x00603640) does
// and counts the nodes with a scheme list, stopping at the first without one.
int Unknown_00603D50()
{
    Scheme_Table();
    UnknownSchemeListIterator it;
    int count = 0;
    while (it.Next() != 0) {
        count++;
    }
    return count;
}
