// 0x00784a3c: function not yet identified
//
// Body: 11 bytes (frozen inventory bound).
// Referenced from 2 sites (unconditional call).
// Ghidra prototype: void Unknown_getc_00784a3c(undefined4 file)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00784a3c {

extern unsigned int  fgetc_00784a0b(unsigned int *file);

void  Unknown_getc_00784a3c(unsigned int file)
{
    fgetc_00784a0b((unsigned int *)(file));
    return;
}

}  // namespace seed_00784a3c
