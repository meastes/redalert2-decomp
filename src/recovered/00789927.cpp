// 0x00789927: function not yet identified
//
// Body: 9 bytes (frozen inventory bound).
// Referenced from 21 sites (unconditional call).
// Ghidra prototype: int doserrno_00789927(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00789927 {

extern unsigned int  getptd_0078969b(void);

int  doserrno_00789927(void)
{
    int ptd;

    ptd = (int)(getptd_0078969b());
    return ptd + 0xc;
}

}  // namespace seed_00789927
