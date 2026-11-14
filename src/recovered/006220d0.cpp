// 0x006220d0: function not yet identified
//
// Body: 8 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined * __fastcall FUN_006220d0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006220d0 {

extern "C" void* PTR_DAT_007a8620;

unsigned char * __fastcall FUN_006220d0(int index)
{
    return (unsigned char *)((&PTR_DAT_007a8620)[index]);
}

}  // namespace seed_006220d0
