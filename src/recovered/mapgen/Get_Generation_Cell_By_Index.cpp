// 0x0056dbc0: function not yet identified
//
// Body: 15 bytes (frozen inventory bound).
// Referenced from 5 sites (unconditional call).
// Ghidra prototype: int __fastcall Get_Generation_Cell_By_Index_0056dbc0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0056dbc0 {

extern "C" unsigned int map_generation_cells_00a70a24;

int __fastcall Get_Generation_Cell_By_Index_0056dbc0(int index)
{
    return index * 0x50 + map_generation_cells_00a70a24;
}

}  // namespace seed_0056dbc0
