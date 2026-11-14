// 0x0056e9a0: function not yet identified
//
// Body: 41 bytes (frozen inventory bound).
// Referenced from 2 sites (unconditional call).
// Ghidra prototype: undefined4 __fastcall unknown_generation_cell_38_0056e9a0(short *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0056e9a0 {

extern "C" unsigned int MapCellStride_0084e804;
extern "C" unsigned int map_generation_cells_00a70a24;

unsigned int __fastcall unknown_generation_cell_38_0056e9a0(short *position)
{
    if (map_generation_cells_00a70a24 == 0) {
        return 0xffffffff;
    }
    return *(unsigned int *)((position[1] * MapCellStride_0084e804 + (int)*position) * 0x50 + 0x38 + map_generation_cells_00a70a24);
}

}  // namespace seed_0056e9a0
