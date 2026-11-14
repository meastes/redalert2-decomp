// 0x0069be30: function not yet identified
//
// Body: 12 bytes (frozen inventory bound).
// Referenced from 2 sites (computed call, data).
// Ghidra prototype: void __fastcall FUN_0069be30(undefined4 param_1,undefined4 *param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0069be30 {

extern unsigned int __fastcall FUN_0069bd60(void);

void __fastcall FUN_0069be30(unsigned int unused_argument, unsigned int *output_value)
{
    unsigned int result_value;

    result_value = (unsigned int)(FUN_0069bd60());
    *output_value = result_value;
    return;
}

}  // namespace seed_0069be30
