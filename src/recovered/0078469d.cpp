// 0x0078469d: function not yet identified
//
// Body: 13 bytes (frozen inventory bound).
// Referenced from 2 sites (unconditional call).
// Ghidra prototype: void srand_0078469d(undefined4 seed)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0078469d {

extern unsigned int  getptd_0078969b(void);

void  srand_0078469d(unsigned int seed)
{
    int ptd;

    ptd = (int)(getptd_0078969b());
    *(unsigned int *)(ptd + 0x14) = seed;
    return;
}

}  // namespace seed_0078469d
