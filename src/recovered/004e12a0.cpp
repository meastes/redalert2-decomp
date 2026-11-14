// 0x004e12a0: function not yet identified
//
// Body: 11 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: bool FUN_004e12a0(void)
// Callees with accepted source:
//   FUN_004e1290 -> src/recovered/004e1290.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004e12a0 {

extern unsigned int  FUN_004e1290(void);

bool  FUN_004e12a0(void)
{
    char result_byte;

    result_byte = (char)(FUN_004e1290());
    return result_byte == '\0';
}

}  // namespace seed_004e12a0
