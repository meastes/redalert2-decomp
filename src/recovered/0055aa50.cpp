// 0x0055aa50: function not yet identified
//
// Body: 41 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void __fastcall FUN_0055aa50(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0055aa50 {

void __fastcall FUN_0055aa50(int map_address)
{
    *(int *)(map_address + 0x118) = *(int *)(map_address + 0xf8) + 1 + *(int *)(map_address + 0xfc);
    *(int *)(map_address + 0x11c) =
              (*(int *)(map_address + 0xf0) - *(int *)(map_address + 0xf8)) + *(int *)(map_address + 0xfc);
    return;
}

}  // namespace seed_0055aa50
