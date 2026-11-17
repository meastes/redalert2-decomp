// 0x007905e2: function not yet identified
//
// Body: 9 bytes (frozen inventory bound).
// Referenced from 17 sites (computed call, data).
// Ghidra prototype: void fptrap_007905e2(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_007905e2 {

extern "C" unsigned int __cdecl __amsg_exit(...);

void  fptrap_007905e2(void)
{
    __amsg_exit(2);
    return;
}

}  // namespace seed_007905e2
