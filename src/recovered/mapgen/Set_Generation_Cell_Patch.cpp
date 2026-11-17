// 0x0056e9d0: function not yet identified
//
// Body: 33 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall Set_Generation_Cell_Patch_0056e9d0(short *param_1,undefined4 param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0056e9d0 {

extern "C" unsigned int MapCellStride_0084e804;
extern "C" unsigned int map_generation_cells_00a70a24;

void __fastcall Set_Generation_Cell_Patch_0056e9d0(short *position, unsigned int patch)
{
    *(unsigned int *)((position[1] * MapCellStride_0084e804 + (int)*position) * 0x50 + 0x3c + map_generation_cells_00a70a24) =
              patch;
    return;
}

}  // namespace seed_0056e9d0
