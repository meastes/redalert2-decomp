// 0x0056db40: function not yet identified
//
// Body: 33 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall Clear_Generation_Cell_Flag_0056db40(short *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0056db40 {

extern "C" unsigned int MapCellStride_0084e804;
extern "C" unsigned int map_generation_cells_00a70a24;

void __fastcall Clear_Generation_Cell_Flag_0056db40(short *position)
{
    *(unsigned char *)((position[1] * MapCellStride_0084e804 + (int)*position) * 0x50 + 0x45 + map_generation_cells_00a70a24) = 0;
    return;
}

}  // namespace seed_0056db40
