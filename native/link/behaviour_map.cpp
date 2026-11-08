// Behaviour-written bodies of unmatched MapClass rows on the start-up path.
// NOT EXACT: the rows stay unrecovered in the exact tree.
#include "shared/map.hpp"

// 0x0055ACC0 (shared/map.hpp: "increments unknown_1154", GScreenClass's
// Flag_To_Redraw counter; the row's leaf src/recovered/0055acc0.cpp adds one
// to the byte at +0x1154).
void MapClass::Unknown_0055ACC0()
{
    *(unsigned char *)&unknown_1154 = (unsigned char)(*(unsigned char *)&unknown_1154 + 1);
}
