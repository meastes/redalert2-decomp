// 0x0078991e: function not yet identified
//
// Body: 9 bytes (frozen inventory bound).
// Referenced from 42 sites (unconditional call).
// Ghidra prototype: int errno_0078991e(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0078991e {

extern unsigned int  getptd_0078969b(void);

int  errno_0078991e(void)
{
    int ptd;

    ptd = (int)(getptd_0078969b());
    return ptd + 8;
}

}  // namespace seed_0078991e
