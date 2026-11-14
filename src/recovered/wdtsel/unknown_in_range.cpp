// 0x00730360: function not yet identified
//
// Body: 14 bytes (frozen inventory bound).
// Referenced from 15 sites (computed call, data).
// Ghidra prototype: bool __fastcall unknown_in_range_00730360(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00730360 {

bool __fastcall unknown_in_range_00730360(int self)
{
    return *(unsigned int *)(self + 8) < *(unsigned int *)(*(int *)(self + 4) + 0x10);
}

}  // namespace seed_00730360
